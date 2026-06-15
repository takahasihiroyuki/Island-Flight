#include "stdafx.h"
#include "GamePlayState.h"
#include "Aircraft.h"
#include "Timer.h"
#include "TimerUI.h"
#include "UIManager.h"
#include "CoinDirectionArrowUI.h"
#include "CoinCounterUI.h"
#include "CoinManager.h"
#include "Stage.h"
#include"ScorePopupScreen.h"
#include "ComboCounterScreen.h"
#include"BonusItemManager.h"

namespace
{
	static constexpr float AIRCRAFT_BASE_THRUST = 10000.0f;
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
	//プレイヤーアシストシステム初期化
	m_playerAssistSystem.Init(m_context->stage->GetAssistWarpPoints());


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

	//ボーナスアイテムマネージャーに依存関係を渡す
	if (m_context->bonusItemManager != nullptr)
	{
		m_context->bonusItemManager->SetGameTimer(
			m_timer
		);
	}

	//UI
	//タイマーUIをUImanagerに登録
	m_timeUI = std::make_unique<TimerUI>();
	m_timeUI->Init(m_timer);
	UIManager::GetInstance().RegisterScreen("timerUI", std::move(m_timeUI));
	UIManager::GetInstance().ShowScreen("timerUI");

	m_coinArrowUI = std::make_unique<CoinDirectionArrowUI>(m_context->coinManager, m_context->aircraft);
	m_coinArrowUI->Init();
	UIManager::GetInstance().RegisterScreen("coinArrowUI", std::move(m_coinArrowUI));
	UIManager::GetInstance().ShowScreen("coinArrowUI");

	m_coinCounterUI = std::make_unique<CoinCounterUI>();
	m_coinCounterUI->Init();
	UIManager::GetInstance().RegisterScreen("coinCounterUI", std::move(m_coinCounterUI));
	UIManager::GetInstance().ShowScreen("coinCounterUI");

	//スコアポップアップUIをUImanagerに登録
	{
		auto scoreAddPopupUI = std::make_unique<ScorePopupScreen>();
		scoreAddPopupUI->Init();

		m_scorePopupScreen = scoreAddPopupUI.get();

		UIManager::GetInstance().RegisterScreen(
			"scorePopupScreen",
			std::move(scoreAddPopupUI)
		);

		UIManager::GetInstance().ShowScreen("scorePopupScreen");

		// CoinManagerにScorePopupScreenを渡す
		auto* coinManager = m_context->coinManager;

		if (coinManager != nullptr)
		{
			coinManager->SetScorePopupScreen(m_scorePopupScreen);
		}
	}

	{
		auto comboCounterScreen = std::make_unique<ComboCounterScreen>();

		auto* scoreManager = m_context->scoreManager;

		comboCounterScreen->Init(scoreManager);

		m_comboCounterScreen = comboCounterScreen.get();

		UIManager::GetInstance().RegisterScreen(
			"comboCounterScreen",
			std::move(comboCounterScreen)
		);

		UIManager::GetInstance().ShowScreen("comboCounterScreen");
	}

	//BGM
	m_gamePlayBGM = NewGO<SoundSource>(0);
	m_gamePlayBGM->Init(static_cast<int>(SoundID::enGamePlayBGM));
	m_gamePlayBGM->Play(true);
}

void GamePlayState::Update()
{
	PlayerInput();
	m_playerAssistSystem.Update(*m_context->aircraft);
	if (m_context->bonusItemManager != nullptr)
	{
		m_context->bonusItemManager->Update();
	}
	m_context->aircraft->Update();
	m_context->coinManager->Update(*m_context->aircraft);

	switch (m_phase)
	{
	case GamePlayPhase::GamePlay:
		if (m_timer->IsTimeUp())
		{
			//アウトロへ
			m_phase = GamePlayPhase::Outro;
			//タイマーをリセットして、アウトロの時間計測開始
			m_timer->Reset();
			m_timer->SetRunning(true);
			//画面をフェードアウト
			g_renderingEngine->GetPostEffect().SetFadeEnabled(true);
			g_renderingEngine->GetPostEffect().StartFadeOut(m_outroFinishTime, FADE_COLLAR);
			//BGMをフェードアウト
			m_gamePlayBGM->SetFadeOut(m_outroFinishTime);
			//UIを消す
			UIManager::GetInstance().RequestUnregisterScreen("timerUI");
			UIManager::GetInstance().RequestUnregisterScreen("coinArrowUI");
			UIManager::GetInstance().RequestUnregisterScreen("coinCounterUI");
			UIManager::GetInstance().RequestUnregisterScreen("PlayerAssistUI");
		}
		break;
	case GamePlayPhase::Outro:
		if (m_timer->GetElapsedTime() > m_outroFinishTime)
		{
			//終了処理へ
			m_isChangeToResult = true;
		}
		break;
	default:
		break;
	}

	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_context->aircraft->GetPosition() + Vector3(0.0f, 50.0f, 0.0f));
	targetSnapshot.SetVelocity(m_context->aircraft->GetLinearVelocity());
	targetSnapshot.SetRotation(m_context->aircraft->GetOrientation());

	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);

}

void GamePlayState::Exit()
{
	m_gamePlayBGM->Stop();

	if (m_context->coinManager != nullptr)
	{
		m_context->coinManager->SetScorePopupScreen(nullptr);
	}

	if (m_context->bonusItemManager != nullptr)
	{
		m_context->bonusItemManager->SetGameTimer(
			nullptr
		);
	}

	UIManager::GetInstance().RequestUnregisterScreen("scorePopupScreen");
	UIManager::GetInstance().RequestUnregisterScreen("comboCounterScreen");
	m_comboCounterScreen = nullptr;
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

void GamePlayState::PlayerInput()
{
	float mainLeftInput = -g_pad[0]->IsPress(enButtonLB1);
	float mainRightInput = g_pad[0]->IsPress(enButtonRB1);
	float tailInput = g_pad[0]->GetLStickYF();
	float verticalInput = g_pad[0]->GetLStickXF();
	bool isBoostOn = g_pad[0]->IsPress(enButtonB);
	bool isThrottleCut = g_pad[0]->IsPress(enButtonA);

	m_context->aircraft->SetControlInputs(
		mainLeftInput,
		mainRightInput,
		tailInput,
		verticalInput,
		isBoostOn,
		isThrottleCut
	);
}
