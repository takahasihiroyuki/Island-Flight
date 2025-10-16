#include "stdafx.h"
#include "Aircraft.h"
#include "LiftingSurface.h"

namespace
{
	const float MAX_THROTLEINPUT = 1.0f; // 最大推力
	const Vector3 INIT_POSITION = Vector3(0.0f, 0.0f, 0.0f); // 初期位置
}

Aircraft::Aircraft()
{
	m_position = INIT_POSITION;
}
bool Aircraft::Start()
{
	m_engine = std::make_unique<Engine>();
	return true;
}
void Aircraft::Init(const char* filePath)
{
	// モデルの初期化
	m_model.Init(filePath, nullptr, 0, enModelUpAxisZ, false);

	m_characterController.Init(100, 100, m_position);

	InitOrientation();

	InitWingPositionOffset();

	InitAllLiftingSurfaces();
}

void Aircraft::Update()
{
	ApplyControlInputs();
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		m_wings[i]->UpdateControlSurface();
		m_wings[i]->UpdateOrientation(m_state.orientation);
	}

	m_engine->UpdateOrientation(m_state.orientation);

	Move();

	ComputeMoment();

	UpdateRelWind();

	UpdateModel();
}
void Aircraft::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
void Aircraft::InitLiftingSurface(
	WingType wingsType,
	Quaternion orientation,
	bool isMirroed,
	Vector3 momentArm,
	float maxWingDeflectionAngle
)
{
	m_wings[static_cast<int>(wingsType)] = new LiftingSurface(orientation, isMirroed, maxWingDeflectionAngle, momentArm);
}


void Aircraft::InitOrientation()
{
	m_initWingsOrientation[static_cast<int>(WingType::MainLeft)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::MainRight)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::Tail)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::Vertical)].SetRotationZ(3.1415 * 2 / 4);
}

void Aircraft::InitWingPositionOffset()
{
	m_wingPositionOffset[static_cast<int>(WingType::MainLeft)] = Vector3(-3.5f, 0.0f, 0.20f);
	m_wingPositionOffset[static_cast<int>(WingType::MainRight)] = Vector3(3.5f, 0.0f, 0.20f);
	m_wingPositionOffset[static_cast<int>(WingType::Tail)] = Vector3(0.0f, 0.0f, 3.00f);
	m_wingPositionOffset[static_cast<int>(WingType::Vertical)] = Vector3(0.0f, 1.20f, 3.00f);
}

void Aircraft::InitAllLiftingSurfaces()
{
	// 左右・上下の翼を順に初期化
	// （例）4枚構成：左主翼・右主翼・水平尾翼・垂直尾翼

	// 主翼（左）
	InitLiftingSurface(
		WingType::MainLeft,
		m_initWingsOrientation[static_cast<int>(WingType::MainLeft)],
		false,
		m_wingPositionOffset[static_cast<int>(WingType::MainLeft)],
		3.1415f * 2 / 18
	);

	// 主翼（右）
	InitLiftingSurface(
		WingType::MainRight,
		m_initWingsOrientation[static_cast<int>(WingType::MainRight)],
		true,
		m_wingPositionOffset[static_cast<int>(WingType::MainRight)],
		3.1415f * 2 / 18
	);

	// 水平尾翼
	InitLiftingSurface(
		WingType::Tail,
		m_initWingsOrientation[static_cast<int>(WingType::Tail)],
		false,
		m_wingPositionOffset[static_cast<int>(WingType::Tail)],
		3.1415f * 2 / 18

	);

	// 垂直尾翼
	InitLiftingSurface(
		WingType::Vertical,
		m_initWingsOrientation[static_cast<int>(WingType::Vertical)],
		false,
		m_wingPositionOffset[static_cast<int>(WingType::Vertical)],
		3.1415f * 2 / 18
	);
}

void Aircraft::ApplyControlInputs()
{
	m_wings[static_cast<int>(WingType::MainLeft)]->SetControlInput(g_pad[0]->GetLStickXF());
	m_wings[static_cast<int>(WingType::MainRight)]->SetControlInput(-g_pad[0]->GetLStickXF());
	m_wings[static_cast<int>(WingType::Tail)]->SetControlInput(g_pad[0]->GetLStickYF());
	m_wings[static_cast<int>(WingType::Vertical)]->SetControlInput(g_pad[0]->GetRStickXF());

}

void Aircraft::Move()
{
	Vector3 force = ComputeForce();

	//Vector3 debug;
	//debug = Vector3::Up * 10;

	AddLinearVelocity(((force / m_mass)) * g_gameTime->GetFrameDeltaTime());



	m_position = m_characterController
		.Execute(
			m_linearVelocity,
			g_gameTime->GetFrameDeltaTime()
		);


}
Vector3 Aircraft::ComputeForce()
{
	m_engine->SetThrottleInput(true/*g_pad[0]->IsPress(enButtonA)*/);
	m_engine->UpdateThrustForce();

	Vector3 thrust = m_engine->GetThrustForce();


	Vector3 wingsForce = Vector3::Zero;
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		m_wings[i]->ComputeForces(m_state);
		wingsForce += m_wings[i]->GetForce();
	}

	Vector3 force = thrust + wingsForce;
	force += ComputeGravity();

	return force;
}
void Aircraft::ComputeMoment()
{
	Vector3 totalMoment=Vector3::Zero;
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		totalMoment += m_wings[i]->ComputeMoment();
	}

	//慣性モーメント
	Vector3 inertia = m_inertia;

	//角加速度
	Vector3 angularAcc;
	angularAcc.x = totalMoment.x / inertia.x;
	angularAcc.y = totalMoment.y / inertia.y;
	angularAcc.z = totalMoment.z / inertia.z;

	//角速度
	m_angularVelocity += angularAcc * g_gameTime->GetFrameDeltaTime();

	Vector3 angularAxis = m_angularVelocity;
	angularAxis.Normalize();


	float omega = m_angularVelocity.Length();
	if (omega > 1e-5f) { // 小さすぎる場合は無視
		// 姿勢を更新
		Quaternion deltaQuaternion;
		deltaQuaternion.SetRotation(angularAxis, m_angularVelocity.Length() * g_gameTime->GetFrameDeltaTime());
		m_state.orientation = deltaQuaternion * m_state.orientation;
	}

	float debug = m_angularVelocity.Length();

}
void Aircraft::UpdateModel()
{
	m_model.SetPosition(m_position);
	m_characterController.SetPosition(m_position);
	m_model.SetRotation(m_state.orientation);
	m_model.Update();
}

void Engine::SetThrottleInput(bool input)
{

	float delataTime = g_gameTime->GetFrameDeltaTime();

	if (input)
		m_holdTime += delataTime;
	else
		m_holdTime -= delataTime;

	if (m_holdTime <= 0)				m_holdTime = 0;
	if (m_holdTime >= MAX_THROTLEINPUT)	m_holdTime = MAX_THROTLEINPUT;


	m_throttleInput = m_holdTime / MAX_THROTLEINPUT;

}
