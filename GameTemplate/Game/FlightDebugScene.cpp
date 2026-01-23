#include "stdafx.h"
#include "FlightDebugScene.h"
#include "DebugArrowUI.h"
#include"Aircraft.h"
#include "UIManager.h"

namespace
{
	static constexpr float AIRCRAFT_BASE_THRUST = 10000.0f;
	static const Vector3 INIT_POSITION = Vector3(0.0f,10000.0f,0.0f);
}

FlightDebugScene::~FlightDebugScene()
{
}

bool FlightDebugScene::Start()
{

	//飛行機の初期化
	{
		//飛行機
		m_aircraft = new Aircraft();
		m_aircraft->Init("Assets/modelData/Plane/Plane.tkm", INIT_POSITION, AIRCRAFT_BASE_THRUST);
		//場所をを固定
		m_aircraft->SetLockTranslation(true);
		m_aircraft->Start();
	}

	//デバッグ用矢印の初期化
	{
		const int wingsCount = m_aircraft->GetWingCount();

		//翼の枚数文矢印を確保
		m_debugMomentUI.resize(wingsCount);
		m_debugForceUI.resize(wingsCount);
		m_debugMomentArmUI.resize(wingsCount);

		//デバッグ用のUIを登録
		for (int i = 0; i < DebugMomentArrowUIType::Count; i++)
		{
			//モーメントUI
			auto debugMomentUI = std::make_unique<DebugArrowUI>();
			debugMomentUI->Init(collarType::enYerrow, 0.001);
			debugMomentUI->SetDisplayed(true);
			m_debugMomentUI[i] = debugMomentUI.get();

			//フォースUI
			auto debugForceUI = std::make_unique<DebugArrowUI>();
			debugForceUI->Init(collarType::enRed, 0.0005);
			debugForceUI->SetDisplayed(true);
			m_debugForceUI[i] = debugForceUI.get();

			//モーメントUI
			auto debugMomentArmUI = std::make_unique<DebugArrowUI>();
			debugMomentArmUI->Init(collarType::enBlue, 2.0f);
			debugMomentArmUI->SetDisplayed(true);
			m_debugMomentArmUI[i] = debugMomentArmUI.get();

			UIManager::GetInstance().RegisterScreen("DebugMomentArrowUI" + std::to_string(i), std::move(debugMomentUI));
			UIManager::GetInstance().RegisterScreen("DebugForceArrowUI" + std::to_string(i), std::move(debugForceUI));
			UIManager::GetInstance().RegisterScreen("DebugMomentArmUI" + std::to_string(i), std::move(debugMomentArmUI));
		}
	}

	//カメラ
	{
		TargetSnapshot targetSnapshot;
		targetSnapshot.SetPosition(m_aircraft->GetPosition());
		targetSnapshot.SetVelocity(m_aircraft->GetLinearVelocity());
		targetSnapshot.SetRotation(m_aircraft->GetOrientation());

		CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
		CameraManager::GetInstance().ChangeController(CameraControllerType::enStatic);
	}

	//海
	{
		m_ocean = NewGO<Ocean>(0);
	}

	return true;
}

void FlightDebugScene::Update()
{

	//飛行機の更新
	{
		m_aircraft->Update();

		//プレイヤーの入力
		PlayerInput();
	}

	//デバッグ用矢印UIの更新
	{
		//翼の枚数分更新
		for (int i = 0; i < DebugMomentArrowUIType::Count; i++)
		{
			//モーメントUI
			m_debugMomentUI[i]->UpdateTargetVec(m_aircraft->GetWingMomentWorld(static_cast<WingType>(i))*1000);
			m_debugMomentUI[i]->UpdatePosition(m_aircraft->GetPosition());
			//フォースUI
			m_debugForceUI[i]->UpdateTargetVec(m_aircraft->GetWingForceWorld(static_cast<WingType>(i)));
			m_debugForceUI[i]->UpdatePosition(m_aircraft->GetPosition());
			//モーメントアームUI
			m_debugMomentArmUI[i]->UpdateTargetVec(m_aircraft->GetWingMomentArmWorld(static_cast<WingType>(i)));
			m_debugMomentArmUI[i]->UpdatePosition(m_aircraft->GetPosition());
		}
	}

	//カメラの更新
	{
		TargetSnapshot targetSnapshot;
		targetSnapshot.SetPosition(m_aircraft->GetPosition());
		targetSnapshot.SetVelocity(m_aircraft->GetLinearVelocity());
		targetSnapshot.SetRotation(m_aircraft->GetOrientation());
		CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
	}
}

void FlightDebugScene::Render(RenderContext& rc)
{
	m_aircraft->Render(rc);
	m_ocean->Render(rc);
}

bool FlightDebugScene::RequestChangeScene(SceneType& type)
{
	if (g_pad[0]->IsTrigger(enButtonUp)&&
		g_pad[0]->IsTrigger(enButtonDown)&&
		g_pad[0]->IsTrigger(enButtonLeft)&&
		g_pad[0]->IsTrigger(enButtonRight)
		)
	{
		type = SceneType::InGame;
		return true;
	}
	return false;
}

void FlightDebugScene::PlayerInput()
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


