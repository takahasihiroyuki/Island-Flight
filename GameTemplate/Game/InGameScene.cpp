#include "stdafx.h"
#include "InGameScene.h"
#include "Aircraft.h"
#include"PlacementObject.h"
#include"CoinManager.h"
#include "Coin.h"
#include "ScoreManager.h"
#include"Stage.h"
#include"InGameStateManager.h"

namespace
{
	Vector3 INIT_AIRCRAFT = Vector3(0.0f, 0.0f, -15000.0f);
}

InGameScene::InGameScene()
{
	m_bg.Init("Assets/modelData/ship(kari).tkm", nullptr, 0, enModelUpAxisZ, true);
	m_bg.SetScale(Vector3(10.0f, 10.0f, 10.0f));
	m_bg.Update();
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

InGameScene::~InGameScene()
{
}

bool InGameScene::Start()
{
	m_bg.SetPosition(Vector3::Zero);
	m_bg.Update();


	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(10000.0f);
	m_skyCube->SetPosition({ 0.0f,0.0f,0.0f });
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	m_ocean = NewGO<Ocean>(0);
	m_aircraft = NewGO<Aircraft>(0, "aircraft");
	m_aircraft->Init("Assets/modelData/Plane/Plane.tkm", INIT_AIRCRAFT);
	m_coinManager = NewGO<CoinManager>(0, "coinManager");
	m_scoreManager = NewGO<ScoreManager>(0, "ScoreManager");

	m_coinManager->SetScoreManager(m_scoreManager);
	m_stage = NewGO<Stage>(0);
	m_stage->GetCoinManager(m_coinManager);

	InitInGameContext();
	m_inGameStateManeger = NewGO<InGameStateManager>(0);
	m_inGameStateManeger->SetContext(m_context);

	test.Init("Assets/modelData/gray.DDS", 10, 10);
	test.Update();
	return true;
}

void InGameScene::Update()
{

	m_coinManager->Update(*m_aircraft);

	m_skyCube->SetPosition(Vector3(m_position.x, 0.0f, m_position.z));

	g_camera3D->SetFar(100000);

}

void InGameScene::Render(RenderContext& rc)
{
	m_skyCube->Render(rc);
	//test.Draw(rc);
	m_bg.Draw(rc);
}

bool InGameScene::RequestChangeScene(SceneType& type)
{
	return false;
}

void InGameScene::InitInGameContext()
{
	m_context.aircraft = m_aircraft;
	m_context.coinManager = m_coinManager;
	m_context.scoreManager = m_scoreManager;
}