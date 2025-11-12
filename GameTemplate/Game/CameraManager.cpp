#include "stdafx.h"
//#include "CameraManager.h"
#include "ICameraController.h"
#include "CameraTypes.h"
#include "SpringFollowController.h"
#include "StaticController.h"

namespace
{
	const CameraControllerType INIT_CONTROLLER_TYPE = CameraControllerType::enStatic;
}
CameraManager::CameraManager()
{
	ChangeController(INIT_CONTROLLER_TYPE);
	m_currentState.pos = Vector3::Zero;
	m_currentState.targetPos = Vector3::One;
	m_currentState.up = Vector3::Up;

}

CameraManager::~CameraManager()
{
}

bool CameraManager::Start()
{
	return true;
}

void CameraManager::Update()
{
	TargetSnapshot snap;
	if (m_targetSnapshot.GetValid()) {
		snap = m_targetSnapshot;
	}

	//コントローラー更新
	m_activeController->UpdateState(snap);
	m_currentState = m_activeController->GetState();

	g_camera3D->SetPosition(m_currentState.pos);
	g_camera3D->SetTarget(m_currentState.targetPos);

}

void CameraManager::SetTargetInfo(const TargetSnapshot& info)
{
	m_targetSnapshot = info;
}

void CameraManager::ChangeController(CameraControllerType type)
{
	TargetSnapshot snap;
	snap = m_targetSnapshot;

	switch (type)
	{
	case CameraControllerType::enStatic:
		m_activeController = new StaticController(m_currentState, snap);
		m_activeController->Enter();
		break;
	case CameraControllerType::enSpringFollow:
		m_activeController = new SpringFollowController(m_currentState, snap);
		m_activeController->Enter();
		break;
	default:
		break;
	}
}

CameraState CameraManager::GetCameraState() const
{
	return CameraState();
}
