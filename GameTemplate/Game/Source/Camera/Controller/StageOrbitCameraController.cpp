#include "Source/stdafx.h"
#include "Source/Camera/Controller/StageOrbitCameraController.h"
#include"Source/Gameplay/Time/Timer.h"

void StageOrbitCameraController::OnEnter()
{
	m_timer = NewGO<Timer>(0);
	m_timer->SetRunning(true);
}

void StageOrbitCameraController::UpdateState(const TargetSnapshot& snap)
{

	if (!snap.GetValid())return;

	float t = m_timer->GetElapsedTime();

	float posX = m_orbitRadius * sin(t * m_orbitSpeed);
	float posZ = m_orbitRadius * cos(t * m_orbitSpeed);

	m_cameraState.pos = Vector3(posX, m_posY, posZ);

	Vector3 targetPositon = Vector3::One;
	snap.GetPosition(targetPositon);
	m_cameraState.targetPos = targetPositon;

	m_cameraState.up = g_camera3D->GetUp();


}
