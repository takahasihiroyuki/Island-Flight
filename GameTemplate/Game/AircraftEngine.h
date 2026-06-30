#pragma once

class LiftingSurface;
class AircraftEngine :public IGameObject
{
public:
	AircraftEngine(float baseThrust) :m_baseThrust(baseThrust) {}
	~AircraftEngine() {}
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
		m_worldThrustDir = localthrustDir;

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

	float GetBaseThrust() const {
		return m_baseThrust;
	}

	float GetThrustScale() const {
		return m_thrustForce.Length();
	}

	bool GetIsBoostOn() const {
		return m_isBoostOn;
	}

private:
	float m_baseThrust = 2000.0f;				// 基本推力
	float m_throttleRatio = 0.0f;				// スロットル割合
	float m_throttleSmoothValue = 0.0f;			// 推力を滑らかにするための値
	float m_boostMultiplier = 2.0f;				// ブースト時の推力倍率
	Vector3 m_thrustForce = Vector3::Zero;		// 推力ベクトル
	Vector3 m_localThrustDir = Vector3::AxisZ;	// 機体前方方向（初期値）
	Vector3 m_worldThrustDir = m_localThrustDir;// ワールド座標での推力方向
	bool m_isBoostOn = false;					// ブーストがかかっているか
	bool m_isThrottleCut = false;				// スロットルカットがかかっているか
};

