#include "stdafx.h"
#include "Game.h"


bool Game::Start()
{
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);

	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);

	m_model.Init("Assets/modelData/sample.tkm", nullptr, 0, enModelUpAxisZ);
	// �����_�����O�G���W���̕`�揈�������s�B
	g_renderingEngine->SetDirectionLight(Vector3{ 0.0f,0.0f,-1.0f }, Vector3{ 1.0f,1.0f,1.0f });
	g_renderingEngine->SetAmbientLight(Vector3{ 0.5f,0.5f,0.5f });

	return true;
}
void Game::Update()
{
	// ���X�e�B�b�N(�L�[�{�[�h�FWASD)�ŕ��s�ړ��B
	m_position.x += g_pad[0]->GetLStickXF();
	m_position.y += g_pad[0]->GetLStickYF();

	// �E�X�e�B�b�N(�L�[�{�[�h�F�㉺���E)�ŉ�]�B
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	// �㉺���E�L�[(�L�[�{�[�h�F2, 4, 6, 8)�Ŋg��
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
	//// ���[���h�s����X�V�B
	//m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
void Game::Render(RenderContext& rc)
{
	// �h���[�B
	m_model.Draw(rc);
}