#pragma once
#include "Source/Camera/ICameraController.h"

class Timer;
class StageOrbitCameraController :public ICameraController
{
public:
	StageOrbitCameraController(
		CameraState cameraState,
		TargetSnapshot targetSnapshot
	)
		:ICameraController(cameraState, targetSnapshot)
	{}

	void OnEnter()override;

	void UpdateState(const TargetSnapshot& snap)override;

private:
	Vector3 m_targetOffset = Vector3(0.0f, 0.0, 500.0f);
	Timer* m_timer = nullptr;
	float m_orbitRadius=40000;
	float m_posY=10000;
	float m_orbitSpeed=0.1f;
};

