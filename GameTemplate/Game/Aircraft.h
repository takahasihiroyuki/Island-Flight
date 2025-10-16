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
		//入力値を非線形にして推力に反映。
		m_thrust = std::pow(m_throttleInput, 2) * m_maxThrust;

		// 推力ベクトルを計算（機体の前方方向に推力をかける）
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
	float m_thrust = 0.0f;					// 現在の推力
	float m_maxThrust = 1000.0f;				// 最大推力
	float m_throttleInput = 0.0f;			// スロットル入力
	float m_holdTime = 0.0f;				// 時間保持
	Vector3 m_thrustForce = Vector3::Zero;	// 推力ベクトル
	Vector3 m_localThrustDir = Vector3::AxisZ;	// 機体前方方向（初期値）
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
	/// 相対風を更新
	/// アップデートの最後に呼ぶ
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
	ModelRender m_model;					// モデル

	AircraftState m_state;
	Vector3 m_position;
	std::unique_ptr<Engine> m_engine;			// 所有権付きポインタ
	Vector3 m_linearVelocity = Vector3::Zero;	//機体の並進速度ベクトル（ワールド座標）
	const float m_mass = 10.0f;							//質量
	std::array<LiftingSurface*, static_cast<size_t>(WingType::Count)> m_wings;
	std::array<Quaternion, static_cast<size_t>(WingType::Count)> m_initWingsOrientation;
	std::array<Vector3, static_cast<size_t>(WingType::Count)> m_wingPositionOffset;
	// 飛行機の場合姿勢によってはほぼ変わらないので
	// 各軸の慣性モーメントを定数として定義
	Vector3 m_inertia = { 800.0f, 1000.0f, 900.0f }; //慣性モーメント
	Vector3 m_angularVelocity = Vector3::Zero;
};
