#pragma once
#include "stdafx.h"

enum class CameraControllerType
{
	enStatic,				//固定カメラ。
	enSpringFollow,	//ばねのように追従するカメラ。
	enStageOrbit,
};

struct CameraState {
	Vector3 pos = Vector3::Zero;
	Vector3 targetPos = Vector3::One;
	Vector3 up = Vector3::Up;
	Vector3 velocity = Vector3::Zero;
};


/// <summary>
/// カメラのターゲット情報。
/// それをそのままターゲットやポジションとして使うわけではなく
/// ターゲットを参考にしてコントローラーを動かす。
/// どう使うか（使わないか）決めるのはコントローラー側。
/// </summary>
struct TargetSnapshot {
public:
	void SetPosition(Vector3 position, bool usePos = true)
	{
		hasPos = usePos;
		pos = position;
		if (hasPos)valid = true;
	}
	void SetVelocity(Vector3 velocity, bool useVel = true)
	{
		hasVel = useVel;
		vel = velocity;
		if (hasVel)valid = true;

	}
	void SetRotation(Quaternion rotation, bool useRot = true)
	{
		hasRot = useRot;
		rot = rotation;
		if (hasRot)valid = true;
	}

	bool GetPosition(Vector3& position)const
	{
		if (!hasPos)return false;
		position = pos;
		return true;
	}
	bool GetVelocity(Vector3& velocity)const
	{
		if (!hasVel)return false;
		velocity = vel;
		return true;
	}
	bool GetRotation(Quaternion& rotation)const
	{
		if (!hasRot)return false;
		rotation = rot;
		return true;
	}

	bool GetValid()const
	{
		return valid;
	}

private:
	Vector3 pos = Vector3::Zero;
	Vector3 vel = Vector3::Zero;//ターゲットの速度ベクトル
	Quaternion rot = Quaternion::Identity;
	bool hasPos = false;
	bool hasVel = false;
	bool hasRot = false;
	bool valid = false;
};

struct ICameraControllerSettings {};

struct SpringFollowSettings :ICameraControllerSettings
{
	Vector3 localOffsetPos = { 0.0f, 5.0f, -15.0f }; // ばねカメラのローカルオフセット
	float dampingRate = 0.3f;                        // 減衰率
	float dampingC = 2.0f;                           // 減衰係数
};