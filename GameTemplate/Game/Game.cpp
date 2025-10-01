#include "stdafx.h"
#include "Game.h"


Game::Game()
{
	for (int i = 0; i < 5; i++) {
		m_model[i].Init("Assets/modelData/sample.tkm", nullptr, 0, enModelUpAxisZ, false);

	}
	m_bg.Init("Assets/modelData/bg/bg.tkm", nullptr, 0, enModelUpAxisZ,true);

}

Game::~Game()
{
}

bool Game::Start()
{
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);

	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);

	m_ocean = NewGO<Ocean>(0);
	return true;
}
void Game::Update()
{
	// 左スティック(キーボード：WASD)で平行移動。
	m_position.x += g_pad[0]->GetLStickXF()*10;
	m_position.y += g_pad[0]->GetLStickYF()*10.0f;

	// 右スティック(キーボード：上下左右)で回転。
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	m_cameraPosition.x +=g_pad[0]->GetRStickXF()*5 ;
	m_cameraPosition.y += g_pad[0]->GetRStickYF()*5;

	// 上下左右キー(キーボード：2, 4, 6, 8)で拡大
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_scale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_scale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_scale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_scale.x -= 0.02f;
	}

	for (int i = 0; i < 5; i++) {
		m_model[i].SetPosition(m_position + Vector3{ (float)(i - 2) * 50.0f,0.0f,0.0f });

		m_model[i].SetRotation(m_rotation);
		m_model[i].Update();
	}

	//Aボタンでkカメラを鏡映。

	if (g_pad[0]->IsTrigger(enButtonA)||m_cameraFlag) {
		g_camera3D->SetPosition(ReflectPointAcrossPlane(g_camera3D->GetPosition(), plane));
		g_camera3D->SetTarget(ReflectVectorAcrossPlane(g_camera3D->GetTarget(), plane));
		//g_camera3D->SetUp(ReflectVectorAcrossPlane(g_camera3D->GetUp(), plane));
		//m_cameraFlag = true;
		//if (m_timer >= 10) {
		//	m_cameraFlag = true;
		//}
	}
	else {
		g_camera3D->SetPosition(m_cameraPosition);
		g_camera3D->SetTarget(m_position);
	}




	//// ワールド行列を更新。
	//m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
void Game::Render(RenderContext& rc)
{
	// ドロー。
	for (int i = 0; i < 5; i++){
		m_model[i].Draw(rc);
	}
	//m_bg.Draw(rc);
}