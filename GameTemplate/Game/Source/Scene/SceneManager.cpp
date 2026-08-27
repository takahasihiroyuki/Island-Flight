#include "Source/stdafx.h"
#include "Source/Scene/SceneManager.h"
#include "Source/Scene/IScene.h"
#include "Source/Scene/InGame/InGameScene.h"
#include "Source/Scene/Title/TitleScene.h"
#include "Source/Scene/FlightDebugScene.h"


SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

bool SceneManager::Start()
{
	ChangeScene(INIT_SCENE);

	return true;
}

void SceneManager::Update()
{

	if (m_currentScene->RequestChangeScene(m_nextSceneType))
	{
		ChangeScene(m_nextSceneType);
	}

}

void SceneManager::ChangeScene(SceneType type)
{
	if (m_currentScene) {
		m_currentScene->Exit();
		DeleteGO(m_currentScene);
	}

	switch (type) {
	case SceneType::Title:
		m_currentScene = NewGO<TitleScene>(0);
		break;
	case SceneType::InGame:
		m_currentScene = NewGO<InGameScene>(0);
		break;
	case SceneType::GameResult:
		// m_currentScene = NewGO<GameResultScene>();
		//m_currentScene->Init(this);
		break;
	case SceneType::FlightDebug:
		m_currentScene = NewGO<FlightDebugScene>(0);
		break;
	}

	if (m_currentScene) {
		m_currentScene->Enter();
	}
}
