#include "Source/stdafx.h"
#include "Source/Scene/InGame/State/ResultState.h"
#include "Source/Scene/InGame/State/ResultUI.h"
#include "Source/UI/UIManager.h"
#include"Source/Gameplay/Aircraft/Aircraft.h"

namespace {
	Vector3 CAMERA_TARGET_POS = { 0.0f,0.0f,0.0f };
	Vector3 FADEOUT_COLLAR = { 0.0f,0.0f,0.0f };
}

ResultState::ResultState()
{
}

ResultState::~ResultState()
{
	DeleteGO(m_resultEnterSE);
	m_resultUI.reset();
}

void ResultState::OnEnter()
{
	//UI
	m_resultUI = std::make_unique<ResultUI>(m_context->scoreManager);
	m_resultUI->Init();
	UIManager::GetInstance().RegisterScreen("ResultUI", std::move(m_resultUI));
	UIManager::GetInstance().ShowScreen("ResultUI");

	//サウンド
	m_resultEnterSE = NewGO<SoundSource>(0);
	m_resultEnterSE->Init(static_cast<int>(SoundID::enResultEnterSE));
	m_resultEnterSE->Play(false);

	m_context->aircraft->PropellerSoundStop();

	//カメラのターゲット情報を渡す。
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(CAMERA_TARGET_POS);

	//カメラ
	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
	CameraManager::GetInstance().ChangeController(CameraControllerType::enStageOrbit);

	//フェードイン
	g_renderingEngine->GetPostEffect().StartFadeIn(3);

}

void ResultState::Update()
{
	switch (m_phase) {
	case ResultPhase::WaitingInput:
		//Aボタンが押されたらフェーズを進める
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_phase = ResultPhase::Outro;
			g_renderingEngine->GetPostEffect().SetFadeEnabled(true);
			g_renderingEngine->GetPostEffect().StartFadeOut(3, FADEOUT_COLLAR);
			UIManager::GetInstance().CloseScreen("ResultUI");
			UIManager::GetInstance().RequestUnregisterScreen("ResultUI");
		}
		break;
	case ResultPhase::Outro:
		m_outroElapsedTime += g_gameTime->GetFrameDeltaTime();
		if (m_outroFinishTime <= m_outroElapsedTime) {
			m_phase = ResultPhase::ToNextScene;
		}
		break;
	}
}

void ResultState::Exit()
{
	UIManager::GetInstance().CloseScreen("ResultUI");
}

bool ResultState::RequestChangeState(InGameStateType& type)
{
	if (m_phase == ResultPhase::ToNextScene)
	{
		type = InGameStateType::enEnd;
		return true;
	}

	return false;
}
