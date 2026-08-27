#pragma once
#include"Source/Camera/CameraTypes.h"
class ICameraController
{
public:

	ICameraController(
		CameraState cameraState,
		TargetSnapshot targetSnapshot
	)
		: m_cameraState(cameraState),
		m_targetSnapshot(targetSnapshot)
	{
	}
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

	virtual void Initialize(int parameterSelect) {}

	/// <summary>
	/// targetがワープしたときに呼ばれる。
	/// ワープするときに特殊な処理をしたい場合にオーバーライドする。
	/// </summary>
	/// <param name="snap"></param>
	virtual void OnTargetWarped(const TargetSnapshot& snap) {}

	virtual void SetPosition(Vector3 position) {}


protected:
	CameraState m_cameraState;
	TargetSnapshot m_targetSnapshot;
};

