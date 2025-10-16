#pragma once
#include"AircraftState.h"

class LiftingSurface;
class Engine :public IGameObject
{
public:
	Engine() {}
	~Engine() {}

	void Update() override
	{

	}

	void UpdateThrustForce()
	{
		//���͒l�����`�ɂ��Đ��͂ɔ��f�B
		m_thrust = std::pow(m_throttleInput, 2) * m_maxThrust;

		// ���̓x�N�g�����v�Z�i�@�̂̑O�������ɐ��͂�������j
		m_thrustForce = m_WoldeThrustDir * m_thrust;
	}

	void SetThrottleInput(bool input);

	void UpdateOrientation(Quaternion orientation) {
		Vector3 localthrustDir = m_localThrustDir;
		orientation.Apply(localthrustDir);
		m_WoldeThrustDir = localthrustDir;

	};

	const Vector3& GetThrustForce() const { return m_thrustForce; }

private:
	float m_thrust = 0.0f;					// ���݂̐���
	float m_maxThrust = 1000.0f;				// �ő各��
	float m_throttleInput = 0.0f;			// �X���b�g������
	float m_holdTime = 0.0f;				// ���ԕێ�
	Vector3 m_thrustForce = Vector3::Zero;	// ���̓x�N�g��
	Vector3 m_localThrustDir = Vector3::AxisZ;	// �@�̑O�������i�����l�j
	Vector3 m_WoldeThrustDir = m_localThrustDir;
};

enum class WingType {
	MainLeft,
	MainRight,
	Tail,
	Vertical,
	Count
};

class Aircraft : public IGameObject
{
public:
	Aircraft();
	~Aircraft() {};
	bool Start() override;
	void fly();
	void Init(const char* filePath);
	void Update() override;
	void Render(RenderContext& rc) override;

	Vector3 GetPosition() const { return m_position; }

	void ComputeMoment();
private:
	void InitLiftingSurface(
		WingType wingsType,
		Quaternion orientation,
		bool isMirroed,
		Vector3 momentArm,
		float maxWingDeflectionAngle
	);

	void InitOrientation();
	void InitWingPositionOffset();
	void InitAllLiftingSurfaces();

	void ApplyControlInputs();

	void Move();
	Vector3 ComputeForce();
	void UpdateModel();
	/// <summary>
	/// ���Ε����X�V
	/// �A�b�v�f�[�g�̍Ō�ɌĂ�
	/// </summary>
	void UpdateRelWind() {
		m_state.relWind = m_linearVelocity * -1;
	}

	void AddLinearVelocity(Vector3 linearVelocity)
	{
		m_linearVelocity += linearVelocity;
		m_state.linearVelocity += linearVelocity;
	}

	Vector3 ComputeGravity() {
		const float mass = m_mass;
		const Vector3 gravity(0.0f, -9.81f, 0.0f);
		return gravity * mass;
	}

private:
	CharacterController m_characterController;
	ModelRender m_model;					// ���f��

	AircraftState m_state;
	Vector3 m_position;
	std::unique_ptr<Engine> m_engine;			// ���L���t���|�C���^
	Vector3 m_linearVelocity = Vector3::Zero;	//�@�̂̕��i���x�x�N�g���i���[���h���W�j
	const float m_mass = 10.0f;							//����
	std::array<LiftingSurface*, static_cast<size_t>(WingType::Count)> m_wings;
	std::array<Quaternion, static_cast<size_t>(WingType::Count)> m_initWingsOrientation;
	std::array<Vector3, static_cast<size_t>(WingType::Count)> m_wingPositionOffset;
	// ��s�@�̏ꍇ�p���ɂ���Ă͂قڕς��Ȃ��̂�
	// �e���̊������[�����g��萔�Ƃ��Ē�`
	Vector3 m_inertia = { 800.0f, 1000.0f, 900.0f }; //�������[�����g
	Vector3 m_angularVelocity = Vector3::Zero;
};
