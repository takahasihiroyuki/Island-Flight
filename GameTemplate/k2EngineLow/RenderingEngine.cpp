#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine::RenderingEngine()
	{
		Init();
	}

	RenderingEngine::~RenderingEngine()
	{
		
	}

	void RenderingEngine::Init()
	{
		InitMainRenderTarget();
		InitCopyToframeBufferSprite();
		InitGBuffer();
		InitDefferedLightingSprite();
	}

	void RenderingEngine::InitMainRenderTarget()
	{
		//���C�������_�����O�^�[�Q�b�g�̍쐬
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitCopyToframeBufferSprite()
	{
		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g��������
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyToframeBufferSprite.Init(spriteInitData);
	}


	void RenderingEngine::InitGBuffer()
	{		// �A���x�h�J���[�p�̃^�[�Q�b�g���쐬
		float clearColor[] = { 0.5f,0.5f,0.5f,1.0f };
		m_gBuffer[enGBufferAlbedo].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//�@���p�̃^�[�Q�b�g���쐬
		m_gBuffer[enGBufferNormal].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

		//�X�y�L�����J���[�p�̃^�[�Q�b�g���쐬
		m_gBuffer[enGBufferSpecular].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

	}

	void RenderingEngine::InitDefferedLightingSprite()
	{
		//�f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g��������
		SpriteInitData spriteInitData;
		spriteInitData.m_width = FRAME_BUFFER_W;
		spriteInitData.m_height = FRAME_BUFFER_H;

		// �f�B�t�@�[�h���C�e�B���O�Ŏg�p����e�N�X�`����ݒ�
		spriteInitData.m_textures[enGBufferAlbedo] = &m_gBuffer[enGBufferAlbedo].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferNormal] = &m_gBuffer[enGBufferNormal].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferSpecular] = &m_gBuffer[enGBufferSpecular].GetRenderTargetTexture();


		spriteInitData.m_fxFilePath = "Assets/shader/deferredLighting.fx";

		spriteInitData.m_expandConstantBuffer = &GetSceneLight().GetSceneLight();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetSceneLight().GetSceneLight());

		// �f�B�t�@�[�h�����_�����O�p�̃X�v���C�g��������
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g��G-Buffer�ɕύX���ď�������
		RenderTarget* rts[] = {
			&m_gBuffer[enGBufferAlbedo]   // 0�Ԗڂ̃����_�����O�^�[�Q�b�g
			,& m_gBuffer[enGBufferNormal]   // 1�Ԗڂ̃����_�����O�^�[�Q�b�g
			,& m_gBuffer[enGBufferSpecular] // 2�Ԗڂ̃����_�����O�^�[�Q�b�g
		};

		// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		// �܂Ƃ߂ă��f�������_�[��`��
		for (auto& MobjData : m_modelList)
		{
			MobjData->OnDraw(rc);
		}

		// �����_�����O�^�[�Q�b�g�ւ̏������ݑ҂�
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		// �����_�����O������C�������_�����O�^�[�Q�b�g�ɂ���
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);

		// G-Buffer�̓��e�����ɂ��ăf�B�t�@�[�h���C�e�B���O
		m_diferredLightingSprite.Draw(rc);

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	}


	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		// �t���[���o�b�t�@�ɃZ�b�g
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		m_copyToframeBufferSprite.Draw(rc);

	}

	void RenderingEngine::ObjectClear()
	{
		m_modelList.clear();
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{

		//GBuffer�ւ̃����_�����O
		RenderToGBuffer(rc);

		//�f�B�t�@�[�h���C�e�B���O
		DeferredLighting(rc);

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[
		CopyMainRenderTargetToFrameBuffer(rc);


		//�`�悵���I�u�W�F�N�g���N���A����
		ObjectClear();

	}

}