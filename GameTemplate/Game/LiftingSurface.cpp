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
	bool isMirroed, 
	float maxWingDeflectionAngle)
{
	m_controlSurface.Init(maxWingDeflectionAngle);
	InitOrientation(orientation, isMirroed);

}

LiftingSurface::~LiftingSurface()
{
}

void LiftingSurface::Update()
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

	m_force = lift + drag;
}

float LiftingSurface::ComputeAngleOfAttack(const Vector3& relWind)
{
	//内積はゼロベクトルがあると計算できないので
	//ゼロがある場合ゼロ度を渡す。
	//一応内積の関数ではゼロベクトルが入るとゼロ（９０度）になるようになっているが
	//今回は0度の方がいいのでこれを書いた。
	if (relWind.Length() == 0 || m_wingChordDir.Length() == 0) return 0.0f;

	// 翼の前後方向と相対風のなす角を求める
	Vector3 relWindDir = relWind;//相対風
	relWindDir.Normalize();
	float dot = Dot(relWindDir, m_wingChordDir);
	if (dot < -1) dot = -1;
	if (dot > 1) dot = 1;
	float angle = acosf(dot);

	// 外積を計算して、どちら側に曲がっているかを調べる
	Vector3 cross;
	cross.Cross(m_wingChordDir, relWindDir);
	// 法線と外積のなす角を求める
	float sign = m_wingNormal.Dot(cross);
	if (sign < 0.0f) {
		angle = -angle; // 負の値にする
	}
	return angle+m_controlSurface.GetDelection();
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
	// 揚力係数
	float liftCoefficient = ComputeLiftCoefficient(angleOfAttack);

	//揚力ベクトル
	Vector3 liftForce = m_wingNormal * dynamicPressure * liftCoefficient * m_area;

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

	// 迎角が-15度から15度の範囲でのみ揚力を発生させる
	if (angleOfAttack < -15.0f || angleOfAttack > 15.0f)
		return 0.0f;

	// この関数の傾き、簡易的な関数にしているので傾きは一定（線形）。
	const float slope = 0.1f;

	return  +slope * angleOfAttack;
}

float LiftingSurface::ComputeDragCoefficient(float angleOfAttack) const
{
	//↓揚力係数と抗力係数の関係式

	float dragCofficient =
		pow(ComputeLiftCoefficient(angleOfAttack), 2.0f)
		/ OSWALD_EFFICIENCY * ASPECT_RATIO /** m_controlSurface.GetMaxDeflection()*/;

	return dragCofficient;

}

