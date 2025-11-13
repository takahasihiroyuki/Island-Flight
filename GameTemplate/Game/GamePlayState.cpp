#include "stdafx.h"
#include "GamePlayState.h"
#include "Aircraft.h"
#include "Timer.h"

namespace 
{
	float TIMELIMIT = 100;
}

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

	m_timer = NewGO<Timer>(0);
	m_timer->Init(TIMELIMIT);
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

bool GamePlayState::RequestChangeState(InGameStateType& type)
{
	return false;
}
