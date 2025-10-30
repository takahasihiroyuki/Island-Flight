#include "stdafx.h"
#include "GameManager.h"
#include "SceneManager.h"

bool GameManager::Start()
{
	m_sceneManager = NewGO<SceneManager>(0);
    return true;
}

void GameManager::Update()
{
}

void GameManager::Render(RenderContext& rc)
{
}
