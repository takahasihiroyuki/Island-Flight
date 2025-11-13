#include "stdafx.h"
#include "SceneManager.h"
#include "IScene.h"
#include "InGameScene.h"
#include "TitleScene.h"


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

	if(m_currentScene->RequestChangeScene(m_nextSceneType))
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
	case SceneType::GameResolt:
		// m_currentScene = NewGO<GameResoltScene>();
		//m_currentScene->Init(this);
		break;
	}

	if (m_currentScene) {
		m_currentScene->Enter();
	}
}
