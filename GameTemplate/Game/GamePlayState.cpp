#include "stdafx.h"
#include "GamePlayState.h"
#include "Aircraft.h"
#include "Timer.h"

namespace
{
	constexpr float TIMELIMIT = 5;
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
	m_timer->SetRunning(true);
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

bool GamePlayState::RequestChangeState(InGameStateType& type)
{
	if (m_timer->IsTimeUp())
	{
		type = InGameStateType::enResolt;
		return true;
	}
	return false;
}
