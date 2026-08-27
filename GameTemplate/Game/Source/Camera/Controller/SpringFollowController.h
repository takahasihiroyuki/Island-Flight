#pragma once
#include "Source/Camera/ICameraController.h"

/// <summary>
/// ばねのような追従をするようにカメラを動かすクラス。
/// </summary>
class SpringFollowController :public ICameraController
{
public:

	SpringFollowController(
		CameraState cameraState,
		TargetSnapshot targetSnapshot
	)
		:ICameraController(cameraState, targetSnapshot)
	{
	}

private:
	void OnEnter()override;
	/// <summary>
	/// カメラステートの初期化
	/// 初期値はポジションとアップが前のフレームのカメラと同じ値
	/// ターゲットはスナップショットのターゲット
	/// </summary>
	void InitCameraState();

	Vector3 CalcSpringMove(
		float dampingC,
		float dampingRate,
		const TargetSnapshot& snap,
		Vector3 offsetPos);

	Vector3 CalcWoldeCameraOffsetPos(Quaternion orientation,float dt);

	void UpdateState(const TargetSnapshot& snap)override;

	void OnTargetWarped(const TargetSnapshot& snap)override;

	/// <summary>
	/// 右スティックの回転に応じてカメラを回転させる。
	/// </summary>
	/// <param name="dt"></param>
	Quaternion MakeOrbitQuaternionByRightStick(float dt, Quaternion targetOrientation);

private:
	Vector3 m_localCameraOffsetPos = Vector3(0.0f, 100.0f, -600.0f);
	float m_orbitHorizontalAngle;  // 横回転角
	float m_orbitVerticalAngle;    // 縦回転角
	float m_cameraRotateX;   // 右スティックX
	float m_cameraRotateY;   // 右スティックY
};

