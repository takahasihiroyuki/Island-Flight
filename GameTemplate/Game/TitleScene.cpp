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

	const Vector3 FADEOUT_COLLAR = Vector3(1.0f, 1.0f, 1.0f);
	const Vector3 FADEIN_COLLAR = Vector3(0.0f, 0.0f, 0.0f);
	const float FADEINTIME = 2.0f;

	const Vector3 CAMERA_POS_OFFSET = Vector3(0.0f, 200.0f, -800.0f);
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
	m_aircraft->Start();
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

			//フェーズを進める
			m_phase = TitlePhase::Outro;

			m_aircraft->SetLockPosition(false);

			g_renderingEngine->GetPostEffect().SetFadeEnabled(true);
			g_renderingEngine->GetPostEffect().StartFadeOut(m_outroFinishTime, FADEOUT_COLLAR);

			UIManager::GetInstance().CloseScreen("TitleUI");
			UIManager::GetInstance().RequestUnregisterScreen("TitleUI");

			//BGMフェードアウト
			m_titleBGM->SetFadeOut(m_outroFinishTime);
			m_aircraft->SetSoundFadeOut(m_outroFinishTime);
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
	//m_groundModel.Draw(rc);
}

bool TitleScene::RequestChangeScene(SceneType& type)
{
	if (m_phase == TitlePhase::ToNextScene)
	{
		type = NEXT_SCENE;
		return true;
	}

	return false;
}

void TitleScene::Enter()
{

	g_renderingEngine->GetPostEffect().SetFadeEnabled(true);
	g_renderingEngine->GetPostEffect().StartFadeIn(FADEINTIME, FADEIN_COLLAR);

	//UI
	m_titleUI = std::make_unique<TitleUI>();
	m_titleUI->Init();
	UIManager::GetInstance().RegisterScreen("TitleUI", std::move(m_titleUI));
	UIManager::GetInstance().ShowScreen("TitleUI");

	//BGM
	m_titleBGM = NewGO<SoundSource>(0);
	m_titleBGM->Init(static_cast<int>(SoundID::enTitleBGM));
	m_titleBGM->Play(true);

	//モデル
	m_aircraft = new Aircraft();
	m_aircraft->Init("Assets/modelData/Plane/Plane.tkm", INIT_AIRCRAFT_POS, AIRCRAFT_BASE_THRUST);
	m_aircraft->SetLockPosition(true);

	m_groundModel.Init("Assets/modelData/stage/islandStage/Island_2.tkm");
	m_groundModel.SetPosition(m_aircraft->GetPosition() + Vector3(0, -130, 0));
	m_groundModel.SetScale(Vector3(1.0f, 0.5f, 4.0f));
	m_groundModel.Update();


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
	CameraManager::GetInstance().ChangeController(CameraControllerType::enStatic);
	CameraManager::GetInstance().SetPosition(m_aircraft->GetPosition() + CAMERA_POS_OFFSET);

	g_camera3D->SetFar(1000000);

	m_ocean->SetWaveSpeed(0.04);

	//ポストエフェクト
	g_renderingEngine->GetPostEffect().SetHexagonalBlurEnabled(true);
	g_renderingEngine->GetPostEffect().SetFogEnabled(true);
	g_renderingEngine->GetPostEffect().SetBloomEnabled(true);

}

void TitleScene::Exit()
{
	UIManager::GetInstance().CloseScreen("TitleUI");
	m_titleBGM->Stop();
}

void TitleScene::CameraTargetOffsetUpdate()
{

}
