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
		m_thrustForce = m_forwardDir * m_thrust;
	}

	void SetThrottleInput(bool input);

	const Vector3& GetThrustForce() const { return m_thrustForce; }

private:
	float m_thrust = 0.0f;					// 現在の推力
	float m_maxThrust = 5000.0f;				// 最大推力
	float m_throttleInput = 0.0f;			// スロットル入力
	float m_holdTime = 0.0f;				// 時間保持
	Vector3 m_thrustForce = Vector3::Zero;	// 推力ベクトル
	Vector3 m_forwardDir = Vector3::AxisZ;	// 機体前方方向（初期値）
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
};
