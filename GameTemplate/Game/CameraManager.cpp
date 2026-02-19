#include "stdafx.h"
//#include "CameraManager.h"
#include "ICameraController.h"
#include "CameraTypes.h"
#include "SpringFollowController.h"
#include "StaticController.h"
#include"StageOrbitCameraController.h"

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
	//コントローラーがなければ更新しない
	if (!m_activeController) return;

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
	delete(m_activeController);
	m_activeController = nullptr;

	TargetSnapshot snap;
	snap = m_targetSnapshot;

	switch (type)
	{
	case CameraControllerType::enStatic:
		m_activeController = new StaticController(m_currentState, snap);
		m_activeController->Enter();
		//m_activeController->Initialize(m_cameraParamerTyp);
		break;
	case CameraControllerType::enSpringFollow:
		m_activeController = new SpringFollowController(m_currentState, snap);
		m_activeController->Enter();
		break;
	case CameraControllerType::enStageOrbit:
		m_activeController = new StageOrbitCameraController(m_currentState, snap);
		m_activeController->Enter();
	default:
		break;
	}
}

CameraState CameraManager::GetCameraState() const
{
	return CameraState();
}

void CameraManager::NotifyTargetWarped(const TargetSnapshot& snap)
{
	//
	if (m_activeController)
	{
		m_activeController->OnTargetWarped(snap);
	}
}

void CameraManager::SetPosition(Vector3 position)
{
	m_activeController->SetPosition(position);
}
