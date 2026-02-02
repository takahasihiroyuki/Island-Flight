#pragma once
#include "ICameraController.h"
namespace {
	const Vector3 DEFAULT_STATIC_OFFSET = Vector3(0.0f, 100.0f, 500.0f);
}

class StaticController :public ICameraController
{
public:
	StaticController(
		CameraState cameraState,
		TargetSnapshot targetSnapshot
	)
		:ICameraController(cameraState, targetSnapshot)
	{}

	void UpdateState(const TargetSnapshot& m_targetSnapshot)override {

		
		Vector3 targetPos = Vector3::Zero;
		m_targetSnapshot.GetPosition(targetPos);
		Vector3 pos = g_camera3D->GetPosition();
		m_cameraState.pos = pos;
		m_cameraState.targetPos = targetPos;
	}

	void SetTargetOffset(const Vector3& offset) {
		m_targetOffset = offset;
	}


	//Enumとかでどのパラメーターを使うかをもらう
	void Initialize(int parameterSelect) override
	{
		// どのパラメーターかの情報をつかってParameterManagerからパラメーターを取得する。
		//StaticParameter* param = static_cast<StaticParameter*>(parameter);
		//param.a;
	}

private:
	Vector3 m_targetOffset = Vector3(0.0f, 0.0, 500.0f);
};

