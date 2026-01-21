#include "stdafx.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "TitleUI.h"
#include "UIManager.h"
#include "Aircraft.h"

namespace
{
	const SceneType NEXT_SCENE = SceneType::InGame;
	const Vector3 CAMERA_TARGET_POS_OFFSET = Vector3(0, 50, 0);
	const Vector3 INIT_AIRCRAFT_POS = Vector3(0.0f, 50000.0f, 0.0f);
	constexpr float AIRCRAFT_BASE_THRUST = 8000.0f;

	const Vector3 FADE_COLLAR = Vector3(1.0f, 1.0f, 1.0f);
}

TitleScene::~TitleScene()
{
	DeleteGO(m_titleBGM);
	DeleteGO(m_ocean);
	DeleteGO(m_skyCube);
	delete(m_aircraft);
}

bool TitleScene::Start()
{
	return true;
}

void TitleScene::Update()
{
	switch (m_phase)
	{
	case TitlePhase::WaitingInput:
		WaitingInputPhaseUpdate();

		//Aボタンが押されたら、フェーズを進める
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_phase = TitlePhase::Outro;

			//カメラのコントローラーを変える
			TargetSnapshot targetSnapshot;
			targetSnapshot.SetPosition(m_aircraft->GetPosition() + CAMERA_TARGET_POS_OFFSET);
			targetSnapshot.SetRotation(m_aircraft->GetOrientation());

			CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
			CameraManager::GetInstance().ChangeController(CameraControllerType::enStatic);

			g_renderingEngine->GetPostEffect().SetFadeEnabled(true);
			g_renderingEngine->GetPostEffect().StartFadeOut(m_outroFinishTime, FADE_COLLAR);

			UIManager::GetInstance().HideScreen("TitleUI");
		}

		break;
	case TitlePhase::Outro:
		OutroPhaseUpdate();

		if (m_outroFinishTime <= m_elapsedTime) {
			m_elapsedTime = 0;

			m_phase = TitlePhase::ToNextScene;
		}
		break;
	case TitlePhase::ToNextScene:
		break;
	default:
		break;
	}

}

void TitleScene::WaitingInputPhaseUpdate()
{
	//カメラのターゲット情報更新
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_aircraft->GetPosition() + CAMERA_TARGET_POS_OFFSET);
	targetSnapshot.SetVelocity(m_aircraft->GetLinearVelocity());
	targetSnapshot.SetRotation(m_aircraft->GetOrientation());

	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);

	m_aircraft->Update();
}

void TitleScene::OutroPhaseUpdate()
{
	//タイマーを進める
	m_elapsedTime += g_gameTime->GetFrameDeltaTime();

	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_aircraft->GetPosition() + CAMERA_TARGET_POS_OFFSET);

	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);

	m_aircraft->SetControlInputs(0.0f, 0.0f, -0.5f, 0.0f, true, false);
	m_aircraft->Update();

}

void TitleScene::Render(RenderContext& rc)
{
	m_aircraft->Render(rc);
	m_skyCube->Render(rc);
}

bool TitleScene::RequestChangeScene(SceneType& type)
{
	if (m_phase == TitlePhase::ToNextScene)
	{
		type = NEXT_SCENE;
		return true;
	}

	if (g_pad[0]->IsTrigger(enButtonB)) {
		type = SceneType::FlightDebug;
		return true;
	}

	return false;
}

void TitleScene::Enter()
{

	//UI
	m_titleUI = std::make_unique<TitleUI>();
	m_titleUI->Init();
	m_titleUI->SetDisplayed(true);
	UIManager::GetInstance().RegisterScreen("TitleUI", std::move(m_titleUI));

	//BGM
	m_titleBGM = NewGO<SoundSource>(0);
	m_titleBGM->Init(static_cast<int>(SoundID::enTitleBGM));
	m_titleBGM->Play(true);

	//モデル
	m_aircraft = new Aircraft();
	m_aircraft->Init("Assets/modelData/Plane/Plane.tkm", INIT_AIRCRAFT_POS, AIRCRAFT_BASE_THRUST);

	//空
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(100000.0f);
	m_skyCube->SetPosition(g_camera3D->GetPosition());
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	//海
	m_ocean = NewGO<Ocean>(0);

	//カメラ
	TargetSnapshot targetSnapshot;
	targetSnapshot.SetPosition(m_aircraft->GetPosition());
	targetSnapshot.SetVelocity(m_aircraft->GetLinearVelocity());
	targetSnapshot.SetRotation(m_aircraft->GetOrientation());

	CameraManager::GetInstance().SetTargetInfo(targetSnapshot);
	CameraManager::GetInstance().ChangeController(CameraControllerType::enSpringFollow);

	g_camera3D->SetFar(1000000);

	//ポストエフェクト
	g_renderingEngine->GetPostEffect().SetHexagonalBlurEnabled(true);
	g_renderingEngine->GetPostEffect().SetFogEnabled(true);
	g_renderingEngine->GetPostEffect().SetBloomEnabled(true);

}

void TitleScene::Exit()
{
	UIManager::GetInstance().HideScreen("TitleUI");
	m_titleBGM->Stop();
}
