#pragma once
#include "AircraftState.h"
///エアークラフト（飛行機）クラスが持つ翼クラス
///ひとつの翼は主翼部分と舵面(ControlSurface)（動かして入力によって動かせる部分）を持っている
/// エアーポートクラスではこのクラスのインスタンスを三つ作る。

/// <summary>
/// 舵面
/// 翼の入力によって動かせる面
/// </summary>
struct ControlSurface
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="maxDef">曲がる最大の角度（ラジアン）</param>
	void Init(float maxDef) {
		maxDeflection = maxDef;
	}

	void SetAreaRatio(float ratio) { areaRatio = ratio; }
	void SetControlInput(float input)
	{
		if (input >= 1)controlInput = 1; return;
		if (input <= 0)controlInput = 0; return;
		controlInput = input;
	}
	void SetDeflection(float def) { deflection = def; }

	void UpdateDeflection() {
		deflection = controlInput * maxDeflection;
	}

	float GetAreaRatio() const{ return areaRatio; };
	float GetDelection() const{ return deflection; };
	float GetMaxDeflection()const { return maxDeflection; };

private:
	float areaRatio = 0.25;//この面が翼を占める割合
	float controlInput = 0.0f;   // 操舵入力（スティックなど）
	float deflection = 0.0f;     //実際の舵角（数値的舵面の動き）（ラジアン）
	float maxDeflection = 0.0f;
};


class AircraftState;
class LiftingSurface
{
public:
	LiftingSurface() {};
	LiftingSurface(
		Quaternion orientation,
		bool isMirroed,
		float maxWingDeflectionAngle);
	~LiftingSurface();
	void Update();
	//void Render(RenderContext& rc) override;
	bool Start();

public:

	void SetControlInput(float input) {
		m_controlSurface.SetControlInput(input);
	}

	Vector3 GetForce() const { return m_force; }

	void ComputeForces(const AircraftState& state);

	void InitOrientation(const Quaternion& rot,bool isMirrored)
	{
		m_orientation = rot;
		m_wingChordDir= Vector3::Back;
		m_wingNormal= Vector3::Up;
		m_wingSpanDir= Vector3::Right;

		// 回転を適用して翼方向をワールド空間に変換
		rot.Apply(m_wingChordDir);
		rot.Apply(m_wingNormal);
		rot.Apply(m_wingSpanDir);

		if (isMirrored)
			m_wingSpanDir *= -1.0f;
	}
private:

	/// <summary>
	/// 相対風とwingChordDirの角度（迎角）を計算する。
	/// </summary>
	/// <param name="m_relWind"></param>
	float ComputeAngleOfAttack(const Vector3& relWind);

	float ComputeDynamicPressure(const AircraftState& state);

	/// <summary>
	/// 揚力（法線方向に働く力）を計算する。
	/// </summary>
	/// <param name="angleOfAttack">迎角</param>
	Vector3 ComputeLift(
		const AircraftState& state,
		float dynamicPressure,
		float angleOfAttack);

	Vector3 ComputeDrag(
		const AircraftState& state,
		float dynamicPressure,
		float angleOfAttack);

	/// <summary>
	/// 迎角から揚力係数を計算する。
	/// 与えられた迎角に対して、この翼がどの程度効率的に揚力を発生できるかを示す係数を返す。
	/// 
	/// TODO: 現在は簡易モデルを使用しているが、
	/// 実際の揚力係数データを利用した補間モデルにも置き換え可能。
	/// NOTE: この関数は厳密な精度を必要としないため、現状の近似でも十分。
	/// </summary>
	/// <param name="angleOfAttack">迎角</param>
	/// <returns>揚力係数</returns>
	float ComputeLiftCoefficient(float angleOfAttack) const;

	/// <summary>
	/// 抗力係数
	/// 迎角から抗力係数を計算する。
	/// 揚力係数と抗力係数の関係式を使って
	/// 揚力係数から計算する。
	/// </summary>
	/// <param name="angleOfAttack"></param>
	/// <returns></returns>
	float ComputeDragCoefficient(float angleOfAttack) const;


private:

	Vector3 m_position = Vector3(0.0f, 0.0f, 0.0f);
	Quaternion m_rot = g_quatIdentity;


	Vector3 m_wingChordDir = Vector3::Back; // 翼の前側から後ろへ伸ばしたベクトルの方向（前後方向）
	Vector3 m_wingSpanDir;					// 翼の根本から翼端へ伸ばしたベクトルの方向（左右方向）
	Vector3 m_wingNormal = Vector3::Up;		// 翼の法線
	Vector3 m_force;						// 翼に働く力（揚力＋抗力）
	float m_area = 1.0f;				// 翼面積
	float m_airDensity = 1.225f;		// 空気密度 (kg/m^3) 標準大気条件での値
	ControlSurface m_controlSurface;
	Quaternion m_orientation;
};

