#include "stdafx.h"
#include "LiftingSurface.h"
#include <algorithm>
#include "Aircraft.h"
#include <iostream>

namespace
{
	static constexpr float ASPECT_RATIO = 4.0f;        // �A�X�y�N�g��
	static constexpr float OSWALD_EFFICIENCY = 0.7f;   // �I�Y�����h�����W��
	static constexpr float PI = 3.1415f;               // �~����
}

bool LiftingSurface::Start()
{


	return true;
}

LiftingSurface::LiftingSurface(
	Quaternion orientation,
	bool isMirroed,
	float maxWingDeflectionAngle,
	Vector3 momentArm,
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
	//����
	float dynamicPressure = ComputeDynamicPressure(state);
	//�}�p
	float angleOfAttack = ComputeAngleOfAttack(state.relWind);
	//�g��
	Vector3 lift = ComputeLift(state, dynamicPressure, angleOfAttack);
	//�R��
	Vector3 drag = ComputeDrag(state, dynamicPressure, angleOfAttack);

	m_force = lift + drag;
}

float LiftingSurface::ComputeAngleOfAttack(const Vector3& relWind)
{
	//���ς̓[���x�N�g��������ƌv�Z�ł��Ȃ��̂�
	//�[��������ꍇ�[���x��n���B
	//�ꉞ���ς̊֐��ł̓[���x�N�g��������ƃ[���i�X�O�x�j�ɂȂ�悤�ɂȂ��Ă��邪
	//�����0�x�̕��������̂ł�����������B
	if (relWind.Length() == 0 || m_wingChordDir.Length() == 0) return 0.0f;

	// ���̑O������Ƒ��Ε��̂Ȃ��p�����߂�
	Vector3 relWindDir = relWind;//���Ε�
	relWindDir.Normalize();

	float dot = Dot(relWindDir, m_wingChordDir);
	if (dot < -1) dot = -1;
	if (dot > 1) dot = 1;
	float angle = acosf(dot);

	// �O�ς��v�Z���āA�ǂ��瑤�ɋȂ����Ă��邩�𒲂ׂ�
	Vector3 cross;
	cross.Cross(m_wingChordDir, relWindDir);
	// �@���ƊO�ς̂Ȃ��p�����߂�
	float sign = m_wingNormal.Dot(cross);
	if (sign < 0.0f) {
		angle = -angle; // ���̒l�ɂ���
	}
	return angle + m_controlSurface.GetDelection();
}

float LiftingSurface::ComputeDynamicPressure(const AircraftState& state)
{
	//���x
	float velocity = state.linearVelocity.Length();

	// ����(�P�ʑ̐ς�����̉^���G�l���M�[)
	float dynamicPressure = 0.5f * m_airDensity * std::pow(velocity, 2);

	return dynamicPressure;
}

Vector3 LiftingSurface::ComputeLift(
	const AircraftState& state,
	float dynamicPressure,
	float angleOfAttack)
{

	Vector3 relWind = state.relWind;
	relWind.Normalize();

	// �g�͕����͑��Ε��ƃX�p���̊O��
	Vector3 liftDir;
	liftDir.Cross(relWind, m_wingSpanDir);
	liftDir.Normalize();

	// �g�͌W��
	float liftCoefficient = ComputeLiftCoefficient(angleOfAttack);

	//�g�̓x�N�g��
	Vector3 liftForce = liftDir * dynamicPressure * liftCoefficient * m_area;

	return liftForce;
}

Vector3 LiftingSurface::ComputeDrag(
	const AircraftState& state,
	float dynamicPressure,
	float angleOfAttack)
{
	float dragCoefficient = ComputeDragCoefficient(angleOfAttack);

	Vector3 dragDirection = state.relWind * -1;
	dragDirection.Normalize();

	Vector3 DragForce = dragDirection * dynamicPressure * dragCoefficient * m_area;

	return DragForce;
}

float LiftingSurface::ComputeLiftCoefficient(float angleOfAttack) const
{
	//�̊p�x�𒴂���ƌ����i15�x�j
	constexpr float stall = 0.261799f;
	// ���̊֐��̌X���A�ȈՓI�Ȋ֐��ɂ��Ă���̂ŌX���͈��i���`�j�B
	const float slope = 0.05f;

	// �}�p��-15�x����15�x�͈̔͂ł̂ݗg�͂𔭐�������
	if (angleOfAttack < -stall)	return -slope * angleOfAttack;
	if (angleOfAttack > stall)	return	(-slope * (angleOfAttack - stall)) + (slope * stall);

	return  slope * angleOfAttack;
}

float LiftingSurface::ComputeDragCoefficient(float angleOfAttack) const
{
	//���g�͌W���ƍR�͌W���̊֌W��

	float dragCofficient =
		pow(ComputeLiftCoefficient(angleOfAttack), 2.0f)
		/ (OSWALD_EFFICIENCY * ASPECT_RATIO * PI);

	return dragCofficient;

}

void LiftingSurface::UpdateOrientation(Quaternion orientation)
{


	// �����[�J�����i�����p���j
	Vector3 localChord = m_localChordDir;
	Vector3 localSpan = m_localSpanDir;
	Vector3 localNormal = m_localNormalDir;
	Vector3 localMomentArm = m_localMomentArm;

	orientation.Apply(localChord);
	orientation.Apply(localSpan);
	orientation.Apply(localNormal);
	orientation.Apply(localMomentArm);

	//�����_�덷�𖳂������ߐ��K���B
	localChord.Normalize();
	localSpan.Normalize();
	localMomentArm.Normalize();


	// �@�̂̎p���N�H�[�^�j�I�����|���ă��[���h�����ɕϊ�
	m_wingChordDir = localChord;
	m_wingSpanDir = localSpan;
	m_wingNormal = localNormal;
	m_worldMomentArm = localMomentArm;
}

