#include "stdafx.h"
#include "Aircraft.h"
#include "LiftingSurface.h"

namespace
{
	const float MAX_THROTLEINPUT = 1.0f; // 最大推力
	const Vector3 INIT_POSITION = Vector3(0.0f, 0.0f, -8000.0f); // 初期位置
	const float CAPSELLE_RADIUS = 100.0f; // カプセルコライダーの半径
	const float CAPSELLE_HEIGHT = 10.0f; // カプセルコライダーの高さ
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

	m_characterController.Init(CAPSELLE_RADIUS, CAPSELLE_HEIGHT, m_position);

	InitOrientation();

	InitWingPositionOffset();

	InitAllLiftingSurfaces();
}

void Aircraft::Update()
{
	m_worldDirty = true;

	ApplyControlInputs();
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		//if (i == 3 /*|| i == 3*/ /*|| i == 1*/) {
		m_wings[i]->UpdateControlSurface();
		m_wings[i]->UpdateOrientation(m_state.orientation);
		//}
	}

	m_engine->UpdateOrientation(m_state.orientation);

	Move();

	ComputeMoment();

	UpdateRelWind();

	UpdateModel();


	//カメラ設定
	//TODO:後でちゃんと他のクラスでかく。
	Vector3 camPosL = Vector3(0.0f, 300.0f, -500.0f); // 機体の後方・上
	Vector3 camTgtL = Vector3(0.0f, 100.0f, 0.0f); // 機体の少し前方上
	Vector3 upL = Vector3::Up;                     // 機体ローカルUp
	m_state.orientation.Apply(camPosL);
	//m_state.orientation.Apply(camTgtL);
	//m_state.orientation.Apply(upL);

	Vector3 camPosW = GetPosition() + camPosL;
	Vector3 camTgtW = GetPosition() + camTgtL;
	Vector3 upW = upL;
	upW.Normalize();

	g_camera3D->SetPosition(camPosW);
	g_camera3D->SetTarget(camTgtW);
	g_camera3D->SetUp(upW);



}
void Aircraft::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
void Aircraft::InitLiftingSurface(
	WingType wingsType,
	Quaternion orientation,
	Vector3 momentArm,
	float maxWingDeflectionAngle,
	bool isMirroed,
	bool isVertical
)
{
	m_wings[static_cast<int>(wingsType)] = new LiftingSurface(orientation, maxWingDeflectionAngle, momentArm, isMirroed, isVertical);
}


void Aircraft::InitOrientation()
{
	m_initWingsOrientation[static_cast<int>(WingType::MainLeft)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::MainRight)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::Tail)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::Vertical)] = Quaternion::Identity;
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

	// 主翼（左）
	InitLiftingSurface(
		WingType::MainLeft,
		m_initWingsOrientation[static_cast<int>(WingType::MainLeft)],
		m_wingPositionOffset[static_cast<int>(WingType::MainLeft)],
		3.1415f * 2 / (18 * 8),
		true
	);

	// 主翼（右）
	InitLiftingSurface(
		WingType::MainRight,
		m_initWingsOrientation[static_cast<int>(WingType::MainRight)],
		m_wingPositionOffset[static_cast<int>(WingType::MainRight)],
		(3.1415f * 2) / (18 * 8)
	);

	// 水平尾翼
	InitLiftingSurface(
		WingType::Tail,
		m_initWingsOrientation[static_cast<int>(WingType::Tail)],
		m_wingPositionOffset[static_cast<int>(WingType::Tail)],
		3.1415f * 2 / (18 * 8)

	);

	// 垂直尾翼
	InitLiftingSurface(
		WingType::Vertical,
		m_initWingsOrientation[static_cast<int>(WingType::Vertical)],
		m_wingPositionOffset[static_cast<int>(WingType::Vertical)],
		3.1415f * 2 / (18 * 2),
		false,
		true
	);
}

void Aircraft::ApplyControlInputs()
{
	m_wings[static_cast<int>(WingType::MainLeft)]->SetControlInput(g_pad[0]->GetRStickXF());
	m_wings[static_cast<int>(WingType::MainRight)]->SetControlInput(g_pad[0]->GetRStickXF());
	m_wings[static_cast<int>(WingType::Tail)]->SetControlInput(g_pad[0]->GetLStickYF());
	m_wings[static_cast<int>(WingType::Vertical)]->SetControlInput(g_pad[0]->GetLStickXF());
}

void Aircraft::Move()
{
	Vector3 force = ComputeForce();

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

	Vector3 debug = m_wings[0]->ComputeMoment(m_state) + m_wings[1]->ComputeMoment(m_state);
	K2_LOG("Sum Moment = (%.5f, %.5f, %.5f) |Len=%.5f\n", debug.x, debug.y, debug.z, debug.Length());

	Vector3 wingsForce = Vector3::Zero;
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		m_wings[i]->ComputeForces(m_state);
		wingsForce += m_wings[i]->GetForce();
	}

	Vector3 debugWing0 = m_wings[0]->GetForce();
	Vector3 debugWing1 = m_wings[1]->GetForce();
	Vector3 debug2 = m_wings[0]->GetForce() + m_wings[1]->GetForce();

	Vector3 force = thrust + wingsForce;
	force += ComputeGravity();

	Vector3 thrustNormal = thrust;
	thrustNormal.Normalize();
	Vector3 wingsForceNormal = wingsForce;
	wingsForceNormal.Normalize();

	return force;
}

void Aircraft::ComputeMoment()
{
	//全ての翼のモーメント（world系）を合計
	Vector3 totalMomentWold = ComputeTotalMomentWorld();

	////モーメントの計算はオブジェクト座標系でしたいので
	//// （オイラー方程式をそのまま使えるから）
	//// 求めたモーメントをオブジェクト座標系に変換する。


	////ワールド座標の逆回転行列（オブジェクト行列）を求める。
	////移動は考慮しないので逆回転行列。
	////ワールド行列は直交行列なので転置行列で逆行列になる。
	//Matrix objMat;
	//objMat.MakeRotationFromQuaternion(m_state.orientation);
	//objMat.Transpose();
	//Vector3 totalMomentObj = totalMomentWold;
	//objMat.Apply(totalMomentObj);


	//角加速度
	Vector3 angularAcc = ComputeOmegaDotBody(totalMomentWold);

	float delta = g_gameTime->GetFrameDeltaTime();


	//角速度
	m_angularVelocity = angularAcc * delta;

	Vector3 angularAxis = m_angularVelocity;
	if (m_angularVelocity.Length() > 0.00001) {
		angularAxis.Normalize();
	}

	K2_LOG("angularVelocity: %f, %f, %f\n", m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z);

	float omega = m_angularVelocity.Length();
	if (omega > 1e-5f) { // 小さすぎる場合は無視
		// 姿勢を更新
		Quaternion deltaQuaternion;
		deltaQuaternion.SetRotation(angularAxis, m_angularVelocity.Length() * g_gameTime->GetFrameDeltaTime());
		m_state.orientation = deltaQuaternion * m_state.orientation;
		m_state.orientation.Normalize();
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

Vector3 Aircraft::ComputeTotalMomentWorld()
{
	Vector3 totalMomentWorld = Vector3::Zero;
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		if (m_wings[i]) {
			totalMomentWorld += m_wings[i]->ComputeMoment(m_state);
		}
	}
	Vector3 wing1 = m_wings[0]->ComputeMoment(m_state);
	Vector3 wing2 = m_wings[1]->ComputeMoment(m_state);
	float Leng1 = wing1.Length();
	float Leng2 = wing2.Length();

	//wing1.Normalize();
	//wing2.Normalize();
	Vector3 normalwing1 = wing1;
	Vector3 debug = wing1 + wing2;

	wing1.Normalize();
	wing2.Normalize();
	float debugDot = wing1.Dot(wing2);

	float L1 = wing1.Length();
	float L2 = wing2.Length();
	float cosOpp = (L1 > 0 && L2 > 0) ? Dot(wing1, (wing2 * -1)) / (L1 * L2) : 1.0f;

	Vector3 sum = wing1 + wing2;
	float sumMag = sum.Length();
	return totalMomentWorld;
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
