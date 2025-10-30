#include "stdafx.h"
#include "TitleScene.h"
#include "SceneManager.h"

namespace
{
	const SceneType NEXT_SCENE = SceneType::InGame;
}

bool TitleScene::Start()
{
	return true;
}

void TitleScene::OnUpdate()
{
}

void TitleScene::Render(RenderContext& rc)
{
}

void TitleScene::CheckChangeScene()
{
	if (g_pad[0]->IsPress(enButtonA)) {
		m_sceneManager->RequestChange(NEXT_SCENE);
	}
}
