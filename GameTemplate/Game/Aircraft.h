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
		m_thrustForce = m_forwardDir * m_thrust;
	}

	void SetThrottleInput(bool input);

	const Vector3& GetThrustForce() const { return m_thrustForce; }

private:
	float m_thrust = 0.0f;					// ���݂̐���
	float m_maxThrust = 5000.0f;				// �ő各��
	float m_throttleInput = 0.0f;			// �X���b�g������
	float m_holdTime = 0.0f;				// ���ԕێ�
	Vector3 m_thrustForce = Vector3::Zero;	// ���̓x�N�g��
	Vector3 m_forwardDir = Vector3::AxisZ;	// �@�̑O�������i�����l�j
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
	void Update() override {
		Move();
		UpdateModel();
		UpdateRelWind();
	}
	void Render(RenderContext& rc) override;

	Vector3 GetPosition() const { return m_position; }

private:
	void InitLiftingSurface(
		WingType wingsType,
		Quaternion orientation,
		bool isMirroed,
		float maxWingDeflectionAngle
	);

	void InitOrientation();
	void InitAllLiftingSurfaces();

	void Move();
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
};
