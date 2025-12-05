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
	Vector3 INIT_AIRCRAFT_POS = Vector3(0.0f, 5000.0f, -30000.0f);
	float AIRCRAFT_BASE_THRUST = 5000.0f;
}

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
	DeleteGO(m_stage);
	DeleteGO(m_ocean);
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
	m_aircraft = new Aircraft();
	m_aircraft->Init("Assets/modelData/Plane/Plane.tkm", INIT_AIRCRAFT_POS, AIRCRAFT_BASE_THRUST);
	m_coinManager = NewGO<CoinManager>(0, "coinManager");
	m_scoreManager = NewGO<ScoreManager>(0, "ScoreManager");

	m_coinManager->SetScoreManager(m_scoreManager);
	m_stage = NewGO<Stage>(0);
	m_stage->GetCoinManager(m_coinManager);

	InitInGameContext();
	m_inGameStateManeger = NewGO<InGameStateManager>(0);
	m_inGameStateManeger->SetContext(m_context);

	m_aircraft->Start();

	g_camera3D->SetFar(100000);

	return true;
}

void InGameScene::Update()
{
	PlayerInput();

	m_aircraft->Update();

	m_coinManager->Update(*m_aircraft);

	m_skyCube->SetPosition(g_camera3D->GetPosition());


}

void InGameScene::Render(RenderContext& rc)
{
	m_skyCube->Render(rc);
	m_aircraft->Render(rc);
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

void InGameScene::PlayerInput()
{
	float mainLeftInput = -g_pad[0]->IsPress(enButtonLB1);
	float mainRightInput = g_pad[0]->IsPress(enButtonRB1);
	float tailInput = g_pad[0]->GetLStickYF();
	float verticalInput = g_pad[0]->GetLStickXF();
	bool isBoostOn = g_pad[0]->IsPress(enButtonA);
	bool isThrottleCut = g_pad[0]->IsPress(enButtonB);

	m_aircraft->SetControlInputs(
		mainLeftInput,
		mainRightInput,
		tailInput,
		verticalInput,
		isBoostOn,
		isThrottleCut
	);
}
