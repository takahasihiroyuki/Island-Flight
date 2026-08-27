#include "Source/stdafx.h"
#include "Source/Core/GameManager.h"
#include "Source/Scene/SceneManager.h"
#include "Source/Camera/CameraManager.h"

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
