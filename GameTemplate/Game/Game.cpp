#include "stdafx.h"
#include "Game.h"


Game::Game()
{
	m_model.Init("Assets/modelData/sample.tkm", nullptr, 0, enModelUpAxisZ,false);
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


	return true;
}
void Game::Update()
{
	// 左スティック(キーボード：WASD)で平行移動。
	m_position.x += g_pad[0]->GetLStickXF()*10;
	m_position.z += g_pad[0]->GetLStickYF()*10.0f;

	// 右スティック(キーボード：上下左右)で回転。
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

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

	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.Update();

	//// ワールド行列を更新。
	//m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
void Game::Render(RenderContext& rc)
{
	// ドロー。
	m_model.Draw(rc);
	m_bg.Draw(rc);
}