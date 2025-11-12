#pragma once
#include"CameraTypes.h"
class ICameraController
{
public:

	ICameraController(
		CameraState cameraState,
		TargetSnapshot targetSnapshot
	)
		: m_cameraState(cameraState),
		m_targetSnapshot(targetSnapshot)
	{}
	virtual void UpdateState(const TargetSnapshot& m_targetSnapshot) = 0;

	virtual CameraState GetState() final
	{
		return m_cameraState;
	}

	virtual void Enter()final
	{
		OnEnter();
	}
	virtual void OnEnter() {};

protected:
	CameraState m_cameraState;
	TargetSnapshot m_targetSnapshot;
};

