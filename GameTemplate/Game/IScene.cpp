#include "stdafx.h"
#include "IScene.h"
IScene::IScene()
{
}
IScene::~IScene()
{
}
void IScene::Update()
{
	CheckChangeScene();
	OnUpdate();
}

void IScene::Init(SceneManager* sceneManager)
{
	m_sceneManager = sceneManager;
	OnInit();
}
