#include "stdafx.h"
#include "GamePlayState.h"
#include "Aircraft.h"
#include "Timer.h"
#include "TimerUI.h"
#include "UIManager.h"
#include "CoinDirectionArrowUI.h"

namespace
{
	constexpr float TIMELIMIT = 150;
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
	m_timeUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("timerUI", std::move(m_timeUI));

	m_coinArrowUI = std::make_unique<CoinDirectionArrowUI>(m_context->coinManager, m_context->aircraft);
	m_coinArrowUI->Init();
	m_coinArrowUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("coinArrowUI", std::move(m_coinArrowUI));

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
	UIManager::GetInstance().HideScreen("coinArrowUI");
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
