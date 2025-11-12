#include "stdafx.h"
#include "GamePlayState.h"
#include "Aircraft.h"

GamePlayState::GamePlayState()
{
}

GamePlayState::~GamePlayState()
{
}

void GamePlayState::OnEnter()
{
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_Context->aircraft->GetPosition());
	targetSnapshot.SetVelocity(m_Context->aircraft->GetLinearVelocity());
	targetSnapshot.SetRotation(m_Context->aircraft->GetOrientation());

	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
	CameraManager::GetInstance().ChangeController(CameraControllerType::enSpringFollow);
}

void GamePlayState::Update()
{
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_Context->aircraft->GetPosition());
	targetSnapshot.SetVelocity(m_Context->aircraft->GetLinearVelocity());
	targetSnapshot.SetRotation(m_Context->aircraft->GetOrientation());

	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
}

void GamePlayState::Exit()
{
}

void GamePlayState::Init()
{
}