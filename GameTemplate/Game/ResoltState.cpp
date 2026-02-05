#include "stdafx.h"
#include "ResoltState.h"
#include "ResultUI.h"
#include "UIManager.h"

namespace {
	Vector3 CAMERA_TARGET_POS = { 0.0f,0.0f,0.0f };
}

ResoltState::ResoltState()
{
}

ResoltState::~ResoltState()
{
	DeleteGO(m_resultEnterSE);
	m_resultUI.reset();
}

void ResoltState::OnEnter()
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

	//カメラのターゲット情報を渡す。
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(CAMERA_TARGET_POS);

	//カメラ
	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
	CameraManager::GetInstance().ChangeController(CameraControllerType::enStageOrbit);

	//フェードイン
	g_renderingEngine->GetPostEffect().StartFadeIn(3);

}

void ResoltState::Update()
{
}

void ResoltState::Exit()
{
	UIManager::GetInstance().CloseScreen("ResultUI");
}

bool ResoltState::RequestChangeState(InGameStateType& type)
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		type = InGameStateType::enEnd;
		return true;
	}

	return false;
}
