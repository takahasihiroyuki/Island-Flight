#pragma once
#include "ICameraController.h"

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

		Vector3 pos = Vector3::Zero;
		Quaternion rot = Quaternion::Identity;

		m_targetSnapshot.GetPosition(pos);
		m_targetSnapshot.GetRotation(rot);
		m_cameraState.pos = pos;
		m_cameraState.targetPos = pos + m_targetOffset;
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

