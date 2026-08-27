#pragma once
#include "Source/Camera/ICameraController.h"
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
	{
	}

	void UpdateState(const TargetSnapshot& m_targetSnapshot)override {


		Vector3 targetPos = Vector3::Zero;
		m_targetSnapshot.GetPosition(targetPos);
		m_cameraState.pos = m_cameraPos;
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

	void SetPosition(Vector3 position) override
	{
		m_cameraPos = position;
	}

private:
	Vector3 m_targetOffset = Vector3(0.0f, 0.0, 500.0f);
	Vector3 m_cameraPos = Vector3::Zero;
};

