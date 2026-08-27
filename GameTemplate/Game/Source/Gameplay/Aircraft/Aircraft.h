#pragma once
#include"Source/Gameplay/Aircraft/AircraftPhysicsState.h"
#include"Source/Gameplay/Aircraft/AircraftEngine.h"


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
	~Aircraft();
	bool Start();
	void fly();
	void Init(const char* filePath, Vector3 initPos, float baseThrust = 2000);

	void Update();

	void Render(RenderContext& rc);

	Vector3 GetPosition() const { return m_state.position; }
	Vector3 GetLinearVelocity()const { return m_state.linearVelocity; }

	void SetPosition(const Vector3& position) {
		m_state.position = position;
	}

	Quaternion GetOrientation()const {
		return m_state.orientation;
	}

	int GetWingCount() const { return static_cast<int>(WingType::Count); }

	void SetControlInputs(
		float mainLeftInput,
		float mainRightInput,
		float tailInput,
		float verticalInput,
		bool isBoostOn,
		bool isThrottleCut
	);

	/// <summary>
	/// 翼のモーメントアームをワールド座標で取得
	/// </summary>
	/// <param name="wingType"></param>
	/// <returns></returns>
	Vector3 GetWingMomentWorld(WingType wingType) const;

	/// <summary>
	/// 翼の力をワールド座標で取得
	/// </summary>
	/// <param name="wingType"></param>
	/// <returns></returns>
	Vector3 GetWingForceWorld(WingType wingType) const;

	Vector3 GetWingMomentArmWorld(WingType wingType) const;

	void SetLockPosition(bool isLock)
	{
		m_lockPosition = isLock;
	}

	void SetLockRotation(bool isLock)
	{
		m_lockRotation = isLock;
	}

	void RequestWarp(Vector3 position, Quaternion rotation)
	{
		m_state.position = position;
		m_characterController.SetPosition(position);
		m_state.orientation = rotation;
		m_state.linearVelocity = Vector3::Zero;
		m_state.angularVelocity = Vector3::Zero;
		UpdateModel();
	}

	void PropellerSoundStop() {
		if (m_propellerSound) {
			m_propellerSound->Stop();
		}
	}

	void SetSoundFadeOut(float fadeTime) {
		m_propellerSound->SetFadeOut(fadeTime);
	}

private:
	void InitLiftingSurface(
		WingType wingsType,
		Quaternion orientation,
		Vector3 momentArm,
		float maxWingDeflectionAngle,
		float area = 1.0f,
		bool isMirroed = false,
		bool isVertical = false
	);

	void InitOrientation();
	void InitWingPositionOffset();
	void InitAllLiftingSurfaces();

	/// <summary>
	/// 翼の更新
	/// </summary>
	void UpdateLiftingSurfaces();

	/// <summary>
	/// エンジンの更新
	/// </summary>
	void UpdateEngine();

	/// <summary>
	/// 移動と衝突の解決
	/// </summary>
	void MoveAndResolveCollision();

	/// <summary>
	/// 見た目、音、エフェクトの更新
	/// </summary>
	void UpdatePresentation();


	void UpdateModel();

	void UpdatePropellerSound();


	void PlayEffects();

	void AddLinearVelocity(Vector3 linearVelocity)
	{
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
	void UpdateAngularMotion();

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
		Vector3 Iw(Ix * m_state.angularVelocity.x,
			Iy * m_state.angularVelocity.y,
			Iz * m_state.angularVelocity.z);

		// ジャイロ項
		Vector3 gyro = Cross(m_state.angularVelocity, Iw);

		Vector3 rhs = momentObj - gyro;

		return Vector3(rhs.x / Ix, rhs.y / Iy, rhs.z / Iz);
	}

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	/// <param name="sweepHit">衝突情報</param>
	void OnCollisionHit(const SweepHit& sweepHit);

	/// <summary>
	/// バウンド計算
	/// </summary>
	/// <param name="sweepHit">衝突情報</param>
	void CalculateBounce(const SweepHit& sweepHit);

private:
	CharacterController m_characterController;
	ModelRender m_model;					// モデル
	ModelRender m_propeller;

	AircraftPhysicsState m_state;
	std::unique_ptr<AircraftEngine> m_engine;			// 所有権付きポインタ
	Vector3 m_accel = Vector3::Zero;			//加速度ベクトル（ワールド座標）
	const float m_mass = 10.0f;							//質量
	std::array<std::unique_ptr<LiftingSurface>, static_cast<size_t>(WingType::Count)> m_wings;
	std::array<Quaternion, static_cast<size_t>(WingType::Count)> m_initWingsOrientation;
	std::array<Vector3, static_cast<size_t>(WingType::Count)> m_wingPositionOffset;
	// 飛行機の場合姿勢によってはほぼ変わらないので
	// 各軸の慣性モーメントを定数として定義
	Vector3 m_inertia = { 800.0f, 1000.0f, 900.0f }; //慣性モーメント

	mutable Matrix m_world;							//ワールド行列
	mutable bool m_worldDirty = true;				// ワールド行列が最新かどうか

	Quaternion m_propellerSpin = Quaternion::Identity;
	SoundSource* m_propellerSound = nullptr;
	bool m_lockPosition = false;		//ポジションを固定（デバッグ用の変数）
	bool m_lockRotation = false;		//回転を固定（デバッグ用の変数）
	float m_propellerSoundBaseVolume = 0.6f;// プロペラ音の基本音量
	EffectEmitter* m_speedLineEffect = nullptr;
};
