#pragma once
#include "stdafx.h"

struct AircraftState
{
	Vector3 position = Vector3::Zero;			// �@�̂̏d�S�ʒu�i���[���h���W�j
	Quaternion orientation = Quaternion();		// �@�̂̎p���i���[���h���@�̕ϊ��p�j
	Vector3 linearVelocity = Vector3::Zero;		// ���i���x�x�N�g���i���[���h���W�j
	Vector3 angularVelocity = Vector3::Zero;	// �p���x�x�N�g���i�@�̉�]�̑����j
	float airDensity;							// ��C���x
	Vector3 relWind = Vector3::Zero;			// ���Ε��i�@�̂��猩�����j
	Vector3 up = Vector3::Up;					// �@�̂̏�����x�N�g���i�I�u�W�F�N�g���W�j
};
