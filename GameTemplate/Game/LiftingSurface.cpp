#include "stdafx.h"
#include "LiftingSurface.h"
#include <algorithm>
#include "Aircraft.h"
#include <iostream>

namespace
{
	static constexpr float ASPECT_RATIO = 4.0f;        // アスペクト比
	static constexpr float OSWALD_EFFICIENCY = 0.7f;   // オズワルド効率係数
	static constexpr float PI = 3.1415f;               // 円周率
}

bool LiftingSurface::Start()
{


	return true;
}

LiftingSurface::LiftingSurface(
	Quaternion orientation,
	float maxWingDeflectionAngle,
	Vector3 momentArm,
	bool isMirroed,
	bool isVertical
) :m_localMomentArm(momentArm)
{
	m_controlSurface.Init(maxWingDeflectionAngle);
	InitOrientation(orientation, isMirroed, isVertical);

}

LiftingSurface::~LiftingSurface()
{
}

void LiftingSurface::ComputeForces(const AircraftState& state)
{
	//動圧
	float dynamicPressure = ComputeDynamicPressure(state);
	//迎角
	float angleOfAttack = ComputeAngleOfAttack(state.relWind);
	//揚力
	Vector3 lift = ComputeLift(state, dynamicPressure, angleOfAttack);
	//抗力
	Vector3 drag = ComputeDrag(state, dynamicPressure, angleOfAttack);

	//揚力・抗力の大きさ（デバッグ用）
	float liftMag = lift.Length();
	float dragMag = drag.Length();

	m_force = lift + drag;
}

float LiftingSurface::ComputeAngleOfAttack(const Vector3& relWind)
{
	const float eps = 1e-6f;

	// 長さゼロに近い場合は 0 とみなす
	if (relWind.Length() < eps) return 0.0f;

	// 相対風
	Vector3 relWindDir = relWind;
	relWindDir.Normalize();

	//スパン成分を除去して、コード・法線の張る平面へ投影
	Vector3 windProj = relWindDir - m_wingSpanDir * Dot(relWindDir, m_wingSpanDir);

	float sinTerm = Dot(Cross(m_wingChordDir, windProj), m_wingNormal);
	float cosTerm = Dot(m_wingChordDir, windProj);
	float aoa = atan2f(sinTerm, cosTerm);

	//操舵面
	float defl = m_controlSurface.GetDeFlection();


	return aoa + defl;
}

float LiftingSurface::ComputeDynamicPressure(const AircraftState& state)
{
	//速度
	float velocity = state.linearVelocity.Length();

	// 動圧(単位体積あたりの運動エネルギー)
	float dynamicPressure = 0.5f * m_airDensity * std::pow(velocity, 2);

	return dynamicPressure;
}

Vector3 LiftingSurface::ComputeLift(
	const AircraftState& state,
	float dynamicPressure,
	float angleOfAttack)
{

	Vector3 rw = state.relWind; if (rw.LengthSq() > 1e-12f) rw.Normalize();
	Vector3 n = m_wingNormal;  if (n.LengthSq() > 1e-12f)  n.Normalize();

	// 相対風に直交な n の成分＝揚力方向
	Vector3 liftDir = n + rw * (-Dot(n, rw));
	if (liftDir.LengthSq() < 1e-12f) liftDir = Cross(n, rw); // 退避
	liftDir.Normalize();

	//Vector3 relWind = state.relWind;
	//relWind.Normalize();


	//// 相対風の単位ベクトル
	//Vector3 rw = state.relWind;
	//if (rw.LengthSq() > 1e-12f) rw.Normalize();

	//// 翼法線
	//Vector3 n = m_wingNormal;
	//if (n.LengthSq() > 1e-12f) n.Normalize();

	//// 法線を相対風に直交な平面へ射影（これが揚力方向）
	//Vector3 liftDir = n + rw * (-Dot(n, rw));

	// 揚力方向は相対風とスパンの外積
	//Vector3 liftDir;
	//liftDir.Cross(relWind, m_wingSpanDir);
	//liftDir.Normalize();

	// 揚力係数
	float liftCoefficient = ComputeLiftCoefficient(angleOfAttack);

	//揚力ベクトル
	Vector3 liftForce = liftDir * dynamicPressure * liftCoefficient * m_area;

	return liftForce;
}

Vector3 LiftingSurface::ComputeDrag(
	const AircraftState& state,
	float dynamicPressure,
	float angleOfAttack)
{
	float dragCoefficient = ComputeDragCoefficient(angleOfAttack);

	Vector3 dragDirection = state.relWind;


	dragDirection.Normalize();

	Vector3 DragForce = dragDirection * dynamicPressure * dragCoefficient * m_area;

	return DragForce;
}

float LiftingSurface::ComputeLiftCoefficient(float angleOfAttack) const
{
	//個の角度を超えると減速（15度）
	constexpr float stall = 0.261799f*2;
	// この関数の傾き、簡易的な関数にしているので傾きは一定（線形）。
	const float slope = 0.05f;

	float cl = slope * angleOfAttack;
	float clMax = slope * stall;

	// 迎角が-15度から15度の範囲でのみ揚力を発生させる
	//これをしないと、衝突処理の際に迎角が大きくなりすぎて、
	// 大きな揚力が発生してしまいクラッシュすることがある。
	cl = (cl > clMax) ? clMax :
		(cl < -clMax) ? -clMax :
		cl;
	return cl;
}

float LiftingSurface::ComputeDragCoefficient(float angleOfAttack) const
{
	//NOTE:基本抗力ををこれより小さくすると、なぜかローカル X 軸または Y 軸まわりに
	//ゆっくりと回転する現象が発生する（数値誤差またはモーメント不均衡とかが原因かも）。
	//現状の値 (0.0005f) では安定しているため、修正の優先度は低い。

	constexpr float DragBase = 0.0005f; // 基本抗力係数(迎角が0の時の抗力係数)

	//↓揚力係数と抗力係数の関係式
	float debug = pow(ComputeLiftCoefficient(angleOfAttack), 2.0);
	float dragCofficient =
		pow(ComputeLiftCoefficient(angleOfAttack), 2.0f)
		/ (OSWALD_EFFICIENCY * ASPECT_RATIO * PI);
	dragCofficient += DragBase;

	return dragCofficient;

}

void LiftingSurface::UpdateOrientation(Quaternion orientation)
{
	//誤差を無くすため正規化。
	orientation.Normalize();

	//小数点誤差を無くすため正規化。
	m_localChordDir.Normalize();
	m_localSpanDir.Normalize();
	m_localNormalDir.Normalize();

	Vector3 chordWorld = m_localChordDir;
	orientation.Apply(chordWorld);
	Vector3 spanWorld = m_localSpanDir;
	orientation.Apply(spanWorld);
	Vector3 normalWorld = m_localNormalDir;
	orientation.Apply(normalWorld);
	Vector3 momentArmWorld = m_localMomentArm;
	orientation.Apply(momentArmWorld);

	// 直交再調整
	chordWorld.Normalize();
	// normal = chord × span（右手系）で再生成 → 直交を保証
	normalWorld = Cross(chordWorld, spanWorld);
	normalWorld.Normalize();
	// span = normal × chord で再構築
	spanWorld = Cross(normalWorld, chordWorld);
	spanWorld.Normalize();

	// 機体の姿勢クォータニオンを掛けてワールド方向に変換
	m_wingChordDir = chordWorld;
	m_wingSpanDir = spanWorld;
	m_wingNormal = normalWorld;
	m_worldMomentArm = momentArmWorld;
}

