#include "stdafx.h"
#include "Aircraft.h"
#include "LiftingSurface.h"

namespace
{
	const float MAX_THROTLEINPUT = 1.0f; // �ő各��
	const Vector3 INIT_POSITION = Vector3(0.0f, 0.0f, 0.0f); // �����ʒu
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
	// ���f���̏�����
	m_model.Init(filePath, nullptr, 0, enModelUpAxisZ, false);

	m_characterController.Init(100, 100, m_position);

	InitOrientation();

	InitAllLiftingSurfaces();
}
void Aircraft::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
void Aircraft::InitLiftingSurface(
	WingType wingsType,
	Quaternion orientation,
	bool isMirroed,
	float maxWingDeflectionAngle
)
{
	m_wings[static_cast<int>(wingsType)] = new LiftingSurface(orientation, isMirroed, maxWingDeflectionAngle);
}


void Aircraft::InitOrientation()
{
	m_initWingsOrientation[static_cast<int>(WingType::MainLeft)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::MainRight)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::Tail)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::Vertical)].SetRotationZ(3.1415 * 2 / 4);
}

void Aircraft::InitAllLiftingSurfaces()
{
	// ���E�E�㉺�̗������ɏ�����
	// �i��j4���\���F���嗃�E�E�嗃�E���������E��������

	// �嗃�i���j
	InitLiftingSurface(
		WingType::MainLeft,
		m_initWingsOrientation[static_cast<int>(WingType::MainLeft)],  // �����i�K�v�Ȃ��]��ݒ�j
		false,                 // �~���[�ł͂Ȃ�
		20.0f                  // �ő�Ǌp�i��j
	);

	// �嗃�i�E�j
	InitLiftingSurface(
		WingType::MainRight,
		m_initWingsOrientation[static_cast<int>(WingType::MainRight)],  // ���]
		true,                                              // �~���[����
		20.0f
	);

	// ��������
	InitLiftingSurface(
		WingType::Tail,
		m_initWingsOrientation[static_cast<int>(WingType::Tail)],      // �����i�K�v�ɉ����āj
		false,
		15.0f
	);

	// ��������
	InitLiftingSurface(
		WingType::Vertical,
		m_initWingsOrientation[static_cast<int>(WingType::Vertical)], // �����
		false,
		25.0f
	);
}

void Aircraft::Move()
{
	m_engine->SetThrottleInput(true/*g_pad[0]->IsPress(enButtonA)*/);
	m_engine->UpdateThrustForce();

	Vector3 thrust = m_engine->GetThrustForce();

	m_wings[static_cast<int>(WingType::MainLeft)]->ComputeForces(m_state);

	Vector3 Force = thrust + m_wings[static_cast<int>(WingType::MainLeft)]->GetForce();

	Vector3 debug;
	if (g_pad[0]->IsPress(enButtonA)) {
		/*debug = Vector3::Up * 100;*/
	}
	else {
		debug = Vector3::Zero;
	}

	AddLinearVelocity(((Force / m_mass) + debug) * g_gameTime->GetFrameDeltaTime());




	m_position = m_characterController
		.Execute(
			m_linearVelocity,
			g_gameTime->GetFrameDeltaTime()
		);
}
void Aircraft::UpdateModel()
{
	m_model.SetPosition(m_position);
	m_characterController.SetPosition(m_position);
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
