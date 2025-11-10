#include "stdafx.h"
#include "InGameScene.h"
#include "Aircraft.h"
#include"PlacementObject.h"
#include"CoinManager.h"
#include "Coin.h"
#include "ScoreManager.h"
#include"Stage.h"
#include"InGameStateManager.h"

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
	m_aircraft->Init("Assets/modelData/Plane/Plane.tkm");
	m_coinManager = NewGO<CoinManager>(0, "coinManager");
	m_scoreManager = NewGO<ScoreManager>(0, "ScoreManager");

	m_coinManager->SetScoreManager(m_scoreManager);
	m_aircraft->SetPosition(Vector3::Up * 500);
	m_stage = NewGO<Stage>(0);
	m_stage->GetCoinManager(m_coinManager);

	InitInGameContext();
	m_inGameStateManeger = NewGO<InGameStateManager>(0);
	m_inGameStateManeger->SetContext(m_context);

	return true;
}

void InGameScene::OnUpdate()
{
	m_coinManager->Update(*m_aircraft);

	if (!g_pad[0]->IsPress(enButtonA)) {
		// 左スティック(キーボード：WASD)で平行移動。
		m_position.x += g_pad[0]->GetLStickXF() * 10.0;
		m_position.y += g_pad[0]->GetLStickYF() * 10.0f;


		// 右スティック(キーボード：上下左右)で回転。
		m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
		m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

		m_cameraPosition.x += g_pad[0]->GetRStickXF() * 50;
		m_cameraPosition.y += g_pad[0]->GetRStickYF() * 50;
		m_cameraPosition.z += g_pad[0]->GetLStickYF() * 50.0f;
	}


	//m_skyCube->SetPosition(Vector3(m_position.x, 0.0f, m_position.z));


	g_camera3D->SetFar(100000);
	g_camera3D->SetPosition(m_cameraPosition);
	g_camera3D->SetTarget(m_position + Vector3(0.0f, 100.0f, 300.0f));
	//g_camera3D->SetTarget(m_aircraft->GetPosition() + Vector3(0.0f, 100.0f, 0.0f));
	//g_camera3D->SetPosition(m_aircraft->GetPosition() + Vector3(0.0f, 300.0f, -500.0f));

}

void InGameScene::Render(RenderContext& rc)
{
	m_skyCube->Render(rc);
	m_bg.Draw(rc);
}

void InGameScene::CheckChangeScene()
{

}

void InGameScene::InitInGameContext()
{
	m_context.aircraft = m_aircraft;
	m_context.coinManager = m_coinManager;
	m_context.scoreManager = m_scoreManager;
}
