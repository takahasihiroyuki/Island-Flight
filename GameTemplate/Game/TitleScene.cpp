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

void TitleScene::Update()
{
}

void TitleScene::Render(RenderContext& rc)
{
}

bool TitleScene::RequestChangeScene(SceneType& type)
{
	if (g_pad[0]->IsPress(enButtonA)) {
		type = NEXT_SCENE;
		return true;
	}

	return false;
}

void TitleScene::Init()
{
}
