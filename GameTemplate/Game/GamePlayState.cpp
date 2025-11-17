#include "stdafx.h"
#include "GamePlayState.h"
#include "Aircraft.h"
#include "Timer.h"
#include "TimerUI.h"
#include "UIManager.h"

namespace
{
	constexpr float TIMELIMIT = 10;
}

GamePlayState::GamePlayState()
{
}

GamePlayState::~GamePlayState()
{
	DeleteGO(m_timer);
	m_timeUI.reset();
}

void GamePlayState::OnEnter()
{
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_context->aircraft->GetPosition());
	targetSnapshot.SetVelocity(m_context->aircraft->GetLinearVelocity());
	targetSnapshot.SetRotation(m_context->aircraft->GetOrientation());

	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
	CameraManager::GetInstance().ChangeController(CameraControllerType::enSpringFollow);

	m_timer = NewGO<Timer>(0);
	m_timer->SetLimitTime(TIMELIMIT);
	m_timer->SetRunning(true);

	//タイマーUIをUImanagerに登録
	m_timeUI = std::make_unique<TimerUI>();
	m_timeUI->Init(m_timer);
	UIManager::GetInstance().RegisterScreen("timerUI", std::move(m_timeUI));

}

void GamePlayState::Update()
{
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_context->aircraft->GetPosition());
	targetSnapshot.SetVelocity(m_context->aircraft->GetLinearVelocity());
	targetSnapshot.SetRotation(m_context->aircraft->GetOrientation());

	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);

}

void GamePlayState::Exit()
{
	UIManager::GetInstance().HideScreen("timerUI");
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
