#include "stdafx.h"
#include "Game.h"
#include "Aircraft.h"


Game::Game()
{
	for (int i = 0; i < 5; i++) {
		m_model.Init("Assets/modelData/Plane/Plane.tkm", nullptr, 0, enModelUpAxisZ, false);
	}
	m_bg.Init("Assets/modelData/bg/bg.tkm", nullptr, 0, enModelUpAxisZ, true);
	m_island.Init("Assets/modelData/stage/shima.tkm", nullptr, 0, enModelUpAxisZ, true);

	//m_characterController.Init(50.0f, 50.0f, m_position);




}

Game::~Game()
{
}

bool Game::Start()
{
	m_island.SetPosition({ 300.0f,0.0f,2000.0f });

	m_island.Update();

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(10000.0f);
	m_skyCube->SetPosition({ 0.0f,0.0f,0.0f });
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);


	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);

	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);

	m_ocean = NewGO<Ocean>(0);
	m_aircraft = NewGO<Aircraft>(0, "aircraft");
	m_aircraft->Init("Assets/modelData/Plane/Plane.tkm");
	return true;
}
void Game::Update()
{
	// 左スティック(キーボード：WASD)で平行移動。
	//m_position.y += g_pad[0]->GetLStickXF()*100.0;
	//m_position.z += g_pad[0]->GetLStickYF()*100.0f;


	// 右スティック(キーボード：上下左右)で回転。
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	m_cameraPosition.x += g_pad[0]->GetRStickXF() * 5;
	m_cameraPosition.y += g_pad[0]->GetRStickYF() * 5;

	//// 上下左右キー(キーボード：2, 4, 6, 8)で拡大
	//if (g_pad[0]->IsPress(enButtonUp)) {
	//	m_scale.y += 0.02f;
	//}
	//if (g_pad[0]->IsPress(enButtonDown)) {
	//	m_scale.y -= 0.02f;
	//}
	//if (g_pad[0]->IsPress(enButtonRight)) {
	//	m_scale.x += 0.02f;
	//}
	//if (g_pad[0]->IsPress(enButtonLeft)) {
	//	m_scale.x -= 0.02f;
	//}


	//Aボタンでkカメラを鏡映。

	if (g_pad[0]->IsTrigger(enButtonA) || m_cameraFlag) {
		//g_camera3D->SetPosition(ReflectPointAcrossPlane(g_camera3D->GetPosition(), plane));
		//g_camera3D->SetTarget(ReflectVectorAcrossPlane(g_camera3D->GetTarget(), plane));
		////g_camera3D->SetUp(ReflectVectorAcrossPlane(g_camera3D->GetUp(), plane));
		////m_cameraFlag = true;
		////if (m_timer >= 10) {
		////	m_cameraFlag = true;
		////}

	}
	else {
		g_camera3D->SetPosition(m_aircraft->GetPosition() + Vector3(0.0f, 300.0f, -500.0f));
		g_camera3D->SetTarget(m_aircraft->GetPosition() + Vector3(0.0f, 100.0f, 0.0f));
		//m_skyCube->SetPosition(Vector3(m_position.x,0.0f,m_position.z));
	}

	m_moveSpeed = Vector3(0.0f, 0.0f, 5000.0f);

	angle = 0.5f;
	//Quaternion rotY;

	//rotY.SetRotationX(angle);
	m_rotation.Apply(m_moveSpeed);


	//m_position = m_characterController
	//	.Execute(
	//		m_moveSpeed,
	//		g_gameTime->GetFrameDeltaTime()
	//	);

	for (int i = 0; i < 5; i++) {
		m_model.SetPosition(m_position /*+ Vector3{ (float)(i - 2) * 50.0f,0.0f,0.0f }*/);

		m_model.SetRotation(m_rotation);
		m_model.Update();
	}

	g_camera3D->SetFar(100000);

	//// ワールド行列を更新。
	//m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
void Game::Render(RenderContext& rc)
{
	// ドロー。
	//m_bg.Draw(rc);
	m_skyCube->Render(rc);
	m_island.Draw(rc);
}