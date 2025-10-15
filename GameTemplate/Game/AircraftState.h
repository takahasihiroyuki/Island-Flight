#pragma once
#include "stdafx.h"

struct AircraftState
{
	Vector3 position = Vector3::Zero;			// 機体の重心位置（ワールド座標）
	Quaternion orientation = Quaternion();		// 機体の姿勢（ワールド→機体変換用）
	Vector3 linearVelocity = Vector3::Zero;		// 並進速度ベクトル（ワールド座標）
	Vector3 angularVelocity = Vector3::Zero;	// 角速度ベクトル（機体回転の速さ）
	float airDensity;							// 空気密度
	Vector3 relWind = Vector3::Zero;			// 相対風（機体から見た風）
	Vector3 up = Vector3::Up;					// 機体の上方向ベクトル（オブジェクト座標）
};
