#pragma once
#include "Source/stdafx.h"

struct AircraftPhysicsState
{
	Vector3 position = Vector3::Zero;			// 機体の重心位置（ワールド座標）
	Quaternion orientation = Quaternion::Identity;		// 機体の姿勢（ワールド→機体変換用）
	Vector3 linearVelocity = Vector3::Zero;		// 並進速度ベクトル（ワールド座標）
	Vector3 angularVelocity = Vector3::Zero;	// 角速度ベクトル（機体回転の速さ）
};
