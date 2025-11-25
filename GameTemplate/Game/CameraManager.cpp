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

	//m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	//m_targetRotationX.AddRotationX(g_pad[0]->IsPress(enButtonA) * 0.05f);
	//m_targetRotationX.AddRotationX(-g_pad[0]->IsPress(enButtonB) * 0.05f);
	////m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	//float posoffsetX = g_pad[0]->GetLStickYF() * 10.0f;
	//float posoffsetZ = -g_pad[0]->GetLStickXF() * 10.0f;
	//float posoffsetY = g_pad[0]->GetRStickYF() * 10.0f;

	//Vector3 moveXZ = { posoffsetX ,posoffsetY,posoffsetZ };
	//m_rotation.Apply(moveXZ);


	//m_cameraPosition += moveXZ;

	//Vector3 target{ 100.0f,0.0f,0.0f };

	//m_rotation.Apply(target);
	////m_targetRotationX.Apply(target);

	//g_camera3D->SetPosition(m_cameraPosition * 100.0f);
	//g_camera3D->SetTarget(m_cameraPosition * 100.f + target);


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
