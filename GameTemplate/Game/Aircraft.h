#pragma once
#include"AircraftState.h"

class LiftingSurface;
class Engine :public IGameObject
{
public:
	Engine() {}
	~Engine() {}
	void Update() override {}

	/// <summary>
	/// 推力の更新
	/// </summary>
	void UpdateThrustForce();

	/// <summary>
	/// 推力の方向を更新
	/// </summary>
	/// <param name="orientation"></param>
	void UpdateOrientation(Quaternion orientation) {
		Vector3 localthrustDir = m_localThrustDir;
		orientation.Normalize();
		orientation.Apply(localthrustDir);
		m_WoldeThrustDir = localthrustDir;

	};

	/// <summary>
	/// 加速ブースト入力設定
	/// </summary>
	/// <param name="isPressed"></param>
	void SetBoostInput(bool isPressed) { m_isBoostOn = isPressed; }

	/// <summary>
	/// エンジンを止めるスロットルカット入力設定
	/// </summary>
	/// <param name="isPressed"></param>
	void SetThrottleCut(bool isPressed) { m_isThrottleCut = isPressed; }

	const Vector3& GetThrustForce() const { return m_thrustForce; }

	void SetBaseThrust(float maxThrust) {
		m_baseThrust = maxThrust;
	}

private:
	float m_baseThrust = 2000.0f;				// 最大推力
	float m_throttleRatio = 0.0f;				// スロットル割合
	float m_throttleSmoothValue = 0.0f;			// 推力を滑らかにするための値
	float m_boostMultiplier = 2.0f;				// ブースト時の推力倍率
	Vector3 m_thrustForce = Vector3::Zero;		// 推力ベクトル
	Vector3 m_localThrustDir = Vector3::AxisZ;	// 機体前方方向（初期値）
	Vector3 m_WoldeThrustDir = m_localThrustDir;// ワールド座標での推力方向
	bool m_isBoostOn = false;					// ブーストがかかっているか
	bool m_isThrottleCut = false;				// スロットルカットがかかっているか
};

enum class WingType {
	MainLeft,
	MainRight,
	Tail,
	Vertical,
	Count
};

enum DebugMomentArrowUIType
{
	enMainLeft,
	enMainRight,
	enTail,
	enVertical,
	Count
};

class DebugArrowUI;
class Aircraft
{
public:
	Aircraft();
	~Aircraft() {};
	bool Start();
	void fly();
	void Init(const char* filePath, Vector3 initPos, float baseThrust = 2000);
	void Update();
	void Render(RenderContext& rc);

	Vector3 GetPosition() const { return m_position; }
	Vector3 GetLinearVelocity()const { return m_linearVelocity; }

	void SetPosition(const Vector3& position) {
		m_position = position;
	}

	Quaternion GetOrientation()const {
		return m_state.orientation;
	}
	void SetControlInputs(
		float mainLeftInput,
		float mainRightInput,
		float tailInput,
		float verticalInput,
		bool isBoostOn,
		bool isThrottleCut
	);

private:
	void InitLiftingSurface(
		WingType wingsType,
		Quaternion orientation,
		Vector3 momentArm,
		float maxWingDeflectionAngle,
		bool isMirroed = false,
		bool isVertical = false
	);

	void InitOrientation();
	void InitWingPositionOffset();
	void InitAllLiftingSurfaces();

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


	/// <summary>
	/// 重力を計算
	/// </summary>
	/// <returns>重力</returns>
	Vector3 ComputeGravity() {
		const float mass = m_mass;
		const Vector3 gravity(0.0f, -9.81f, 0.0f);
		//const Vector3 gravity(0.0f, 0.0f, 0.0f);

		return gravity * mass;
	}
	////////////力計算系///////////////////

	/// <summary>
	/// 力計算
	/// </summary>
	/// <returns></returns>
	Vector3 ComputeForce();

	/////////////モーメント計算系///////////////////

	/// <summary>
	/// モーメントを計算
	/// </summary>
	void ComputeMoment();

	/// <summary>
	/// 翼の総モーメントをワールド座標で計算
	/// </summary>
	/// <returns></returns>
	Vector3  ComputeTotalMomentWorld();

	/// <summary>
	/// オイラー方程式を計算
	/// </summary>
	/// <param name="momentObj">オブジェクト座標のモーメント</param>
	/// <returns></returns>
	Vector3 ComputeOmegaDotBody(const Vector3& momentObj) const
	{
		// 主慣性モーメント
		const float Ix = m_inertia.x;
		const float Iy = m_inertia.y;
		const float Iz = m_inertia.z;

		// Iω（対角なら要素積）
		Vector3 Iw(Ix * m_angularVelocity.x,
			Iy * m_angularVelocity.y,
			Iz * m_angularVelocity.z);

		// ジャイロ項
		Vector3 gyro = Cross(m_angularVelocity, Iw);

		Vector3 rhs = momentObj - gyro;

		return Vector3(rhs.x / Ix, rhs.y / Iy, rhs.z / Iz);
	}

private:
	CharacterController m_characterController;
	ModelRender m_model;					// モデル

	AircraftState m_state;
	Vector3 m_position;
	std::unique_ptr<Engine> m_engine;			// 所有権付きポインタ
	Vector3 m_linearVelocity = Vector3::Zero;	//機体の並進速度ベクトル（ワールド座標）
	Vector3 m_accel = Vector3::Zero;			//加速度ベクトル（ワールド座標）
	const float m_mass = 10.0f;							//質量
	std::array<LiftingSurface*, static_cast<size_t>(WingType::Count)> m_wings;
	std::array<Quaternion, static_cast<size_t>(WingType::Count)> m_initWingsOrientation;
	std::array<Vector3, static_cast<size_t>(WingType::Count)> m_wingPositionOffset;
	// 飛行機の場合姿勢によってはほぼ変わらないので
	// 各軸の慣性モーメントを定数として定義
	Vector3 m_inertia = { 800.0f, 1000.0f, 900.0f }; //慣性モーメント
	Vector3 m_angularVelocity = Vector3::Zero;

	mutable Matrix m_world;							//ワールド行列
	mutable bool m_worldDirty = true;				// ワールド行列が最新かどうか

	std::array<DebugArrowUI*, static_cast<int>(WingType::Count)> m_debugMomentUI;
	std::array<DebugArrowUI*, static_cast<int>(WingType::Count)> m_debugForceUI;
	std::array<DebugArrowUI*, static_cast<int>(WingType::Count)> m_debugMomentArm;

};
