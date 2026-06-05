#include "stdafx.h"
#include "InGameScene.h"
#include "Aircraft.h"
#include "PlacementObject.h"
#include "CoinManager.h"
#include "Coin.h"
#include "ScoreManager.h"
#include "Stage.h"
#include "InGameStateManager.h"
#include "GameTuningSettings.h"

namespace
{
	static constexpr float AIRCRAFT_BASE_THRUST = 10000.0f;
	static const Vector3 FOG_COLLAR = Vector3(0.9f, 1.0f, 1.0f);
	static constexpr float FOG_DISTANCE_SCALE = 0.5;
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
	//ゲームチューニングの初期化
	GameTuning::GameTuningSettings tuningSettings = GameTuning::CreateDefaultGameTuningSettings();


	//スカイキューブ
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.02f);
	m_skyCube->SetScale(15000.0f);
	m_skyCube->SetPosition({ 0.0f,0.0f,0.0f });
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	//海
	m_ocean = NewGO<Ocean>(0);

	//スコアマネージャー
	m_scoreManager = NewGO<ScoreManager>(0, "ScoreManager");
	m_scoreManager->Init(tuningSettings.scoreManager);

	//コインマネージャー
	m_coinManager = NewGO<CoinManager>(0, "coinManager");
	m_coinManager->SetScoreManager(m_scoreManager);

	//ステージ
	m_stage = NewGO<Stage>(0);
	m_stage->GetCoinManager(m_coinManager);
	m_stage->Init();
	m_stage->SetFogParams(FOG_COLLAR, FOG_DISTANCE_SCALE);

	//飛行機
	m_aircraft = new Aircraft();
	m_aircraft->Init("Assets/modelData/Plane/Plane.tkm", m_stage->GetPlayerStartPos(), AIRCRAFT_BASE_THRUST);
	m_aircraft->Start();

	//インゲームステートmanager
	InitInGameContext();
	m_inGameStateManeger = NewGO<InGameStateManager>(0);
	m_inGameStateManeger->SetContext(m_context);

	//カメラ
	g_camera3D->SetFar(200000);

	return true;
}

void InGameScene::Update()
{
	PlayerInput();

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

void InGameScene::Enter()
{
	g_renderingEngine->GetPostEffect().StartFadeIn(3);
}

void InGameScene::InitInGameContext()
{
	m_context.aircraft = m_aircraft;
	m_context.coinManager = m_coinManager;
	m_context.scoreManager = m_scoreManager;
	m_context.stage = m_stage;
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
