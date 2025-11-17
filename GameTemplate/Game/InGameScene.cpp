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
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

InGameScene::~InGameScene()
{
	DeleteGO(m_stage);
	DeleteGO(m_ocean);
	DeleteGO(m_aircraft);
	DeleteGO(m_scoreManager);
	DeleteGO(m_inGameStateManeger);
	DeleteGO(m_coinManager);
	DeleteGO(m_skyCube);

}

bool InGameScene::Start()
{

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
}

bool InGameScene::RequestChangeScene(SceneType& type)
{
	if (m_inGameStateManeger->IsEnd())
	{
		type = SceneType::Title;
		DeleteGO(m_inGameStateManeger);
		return true;
	}
	return false;
}

void InGameScene::InitInGameContext()
{
	m_context.aircraft = m_aircraft;
	m_context.coinManager = m_coinManager;
	m_context.scoreManager = m_scoreManager;
}