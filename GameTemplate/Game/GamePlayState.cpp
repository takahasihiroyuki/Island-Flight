#include "stdafx.h"
#include "GamePlayState.h"
#include "Aircraft.h"
#include "Timer.h"
#include "TimerUI.h"
#include "UIManager.h"
#include "CoinDirectionArrowUI.h"
#include "CoinCounterUI.h"

namespace
{
	constexpr float TIMELIMIT = 100;
	const Vector3 FADE_COLLAR = Vector3(1.0f, 1.0f, 1.0f);
}

GamePlayState::GamePlayState()
{
}

GamePlayState::~GamePlayState()
{
	DeleteGO(m_timer);
	m_timeUI.reset();
	DeleteGO(m_gamePlayBGM);
}

void GamePlayState::OnEnter()
{
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_context->aircraft->GetPosition());
	targetSnapshot.SetVelocity(m_context->aircraft->GetLinearVelocity());
	targetSnapshot.SetRotation(m_context->aircraft->GetOrientation());

	//カメラ
	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
	CameraManager::GetInstance().ChangeController(CameraControllerType::enSpringFollow);

	m_timer = NewGO<Timer>(0);
	m_timer->SetLimitTime(TIMELIMIT);
	m_timer->SetRunning(true);

	//UI
	//タイマーUIをUImanagerに登録
	m_timeUI = std::make_unique<TimerUI>();
	m_timeUI->Init(m_timer);
	m_timeUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("timerUI", std::move(m_timeUI));

	m_coinArrowUI = std::make_unique<CoinDirectionArrowUI>(m_context->coinManager, m_context->aircraft);
	m_coinArrowUI->Init();
	m_coinArrowUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("coinArrowUI", std::move(m_coinArrowUI));

	m_coinCounterUI = std::make_unique<CoinCounterUI>();
	m_coinCounterUI->Init();
	m_coinCounterUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("coinCounterUI", std::move(m_coinCounterUI));

	//BGM
	m_gamePlayBGM = NewGO<SoundSource>(0);
	m_gamePlayBGM->Init(static_cast<int>(SoundID::enGamePlayBGM));
	m_gamePlayBGM->Play(true);
}

void GamePlayState::Update()
{
	switch (m_phase)
	{
	case GamePlayPhase::GamePlay:
		if (m_timer->IsTimeUp())
		{
			m_phase = GamePlayPhase::Outro;
			m_timer->Reset();
			m_timer->SetRunning(true);
			g_renderingEngine->GetPostEffect().SetFadeEnabled(true);
			g_renderingEngine->GetPostEffect().StartFadeOut(m_outroFinishTime, FADE_COLLAR);

		}
		break;
	case GamePlayPhase::Outro:
		if(m_timer->GetElapsedTime() > m_outroFinishTime)
		{
			//終了処理へ
			m_isChangeToResult = true;
		}
		break;
	default:
		break;
	}
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
	UIManager::GetInstance().HideScreen("coinCounterUI");
	m_gamePlayBGM->Stop();
}

bool GamePlayState::RequestChangeState(InGameStateType& type)
{
	if (m_isChangeToResult)
	{
		type = InGameStateType::enResolt;
		return true;
	}
	return false;
}
