#include "stdafx.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "TitleUI.h"
#include "UIManager.h"

namespace
{
	const SceneType NEXT_SCENE = SceneType::InGame;
}

TitleScene::~TitleScene()
{
	DeleteGO(m_titleBGM);
}

bool TitleScene::Start()
{
	return true;
}

void TitleScene::Update()
{
}

void TitleScene::Render(RenderContext& rc)
{
}

bool TitleScene::RequestChangeScene(SceneType& type)
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		type = NEXT_SCENE;
		return true;
	}

	return false;
}

void TitleScene::Enter()
{
	m_titleUI = std::make_unique<TitleUI>();
	m_titleUI->Init();
	m_titleUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("TitleUI", std::move(m_titleUI));

	//BGM
	m_titleBGM = NewGO<SoundSource>(0);
	m_titleBGM->Init(static_cast<int>(SoundID::enTitleBGM));
	m_titleBGM->Play(true);

}

void TitleScene::Exit()
{
	UIManager::GetInstance().HideScreen("TitleUI");
	m_titleBGM->Stop();
}
