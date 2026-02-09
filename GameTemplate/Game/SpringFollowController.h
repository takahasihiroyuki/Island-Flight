#pragma once
#include "ICameraController.h"

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
	{}

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

	Vector3 CalcWoldeCameraOffsetPos(Quaternion orientation);

	void UpdateState(const TargetSnapshot& snap)override;

	void OnTargetWarped(const TargetSnapshot& snap)override;

private:
	Vector3 m_localCameraOffsetPos = Vector3(0.0f, 100.0f, -600.0f);
};

