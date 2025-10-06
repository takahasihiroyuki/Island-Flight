#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{

	RenderingEngine::RenderingEngine()
	{
		for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); ++i) {
			auto layer = static_cast<ReflectLayer>(i);

			m_planeReflectionPass.try_emplace(layer);
			m_reflectedModelList.try_emplace(layer);
			m_reflectPlane.try_emplace(layer, Vector3(0, 1, 0), 0.0f);
		}
	}

	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Update()
	{
		//// ���˃J�����̍X�V
		//for (auto it = m_planeReflectionPass.begin(); it != m_planeReflectionPass.end(); ++it) {
		//	ReflectLayer layer = it->first;     // �}�b�v�̃L�[
		//	auto& pass = it->second;
		//	pass.UpdateReflectCamera(m_reflectPlane.at(layer));
		//}

	}

	void RenderingEngine::Init()
	{
		// �c�����������c
		SpriteInitData sd{};
		sd.m_textures[0] = &m_planeReflectionPass[ReflectLayer::enOcean]
			.GetPlanarReflectionTarget()        // �� PlaneReflectionPass����Getter��p��
			.GetRenderTargetTexture(); // �� �J���[��SRV��Ԃ����b�p
		sd.m_width = 512;   // �D���ȃT�C�Y
		sd.m_height = 288;
		sd.m_fxFilePath = "Assets/shader/sprite.fx";
		m_debugReflectionSprite.Init(sd);


		//�V���h�E�}�b�v�̏������B
		m_shadow.Init();

		InitMainRenderTarget();

		InitGBuffer();

		//�f�B���N�V�������C�g�̐ݒ�B
		m_sceneLight.Init();

		//�|�X�g�G�t�F�N�g�̏������B
		m_postEffect.Init(
			m_mainRenderTarget
		);

		InitDefferedLightingSprite();

		InitCopyToframeBufferSprite();

		for (auto it = m_planeReflectionPass.begin(); it != m_planeReflectionPass.end(); ++it) {
			auto& pass = it->second;
			pass.Init();
		}

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
		m_gBuffer[enGBufferAlbedoDepth].Create(
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

		//���[���h���W�p�̃^�[�Q�b�g���쐬
		m_gBuffer[enGBufferWorldPos].Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
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
		spriteInitData.m_textures[enGBufferAlbedoDepth] = &m_gBuffer[enGBufferAlbedoDepth].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferNormal] = &m_gBuffer[enGBufferNormal].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferSpecular] = &m_gBuffer[enGBufferSpecular].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferWorldPos] = &m_gBuffer[enGBufferWorldPos].GetRenderTargetTexture();
		spriteInitData.m_textures[enGBufferShadow] = &m_shadow.GetShadowTarget().GetRenderTargetTexture();


		spriteInitData.m_fxFilePath = "Assets/shader/deferredLighting.fx";

		spriteInitData.m_expandConstantBuffer = &GetSceneLight().GetLight();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetSceneLight().GetLight());

		// �f�B�t�@�[�h�����_�����O�p�̃X�v���C�g��������
		m_diferredLightingSprite.Init(spriteInitData);
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer[enGBufferAlbedoDepth].GetDSVCpuDescriptorHandle()
		);

		for (auto& renderObj : m_forwardModelList) {
			renderObj->OnDraw(rc);
		}

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g��G-Buffer�ɕύX���ď�������
		RenderTarget* rts[] = {
			&m_gBuffer[enGBufferAlbedoDepth]   // 0�Ԗڂ̃����_�����O�^�[�Q�b�g
			,&m_gBuffer[enGBufferNormal]   // 1�Ԗڂ̃����_�����O�^�[�Q�b�g
			,&m_gBuffer[enGBufferSpecular] // 2�Ԗڂ̃����_�����O�^�[�Q�b�g
			,&m_gBuffer[enGBufferWorldPos] // 3�Ԗڂ̃����_�����O�^�[�Q�b�g
		};

		// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		// �܂Ƃ߂ă��f�������_�[��`��
		for (auto& MobjData : m_deferredModelList)
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
		m_deferredModelList.clear();
		m_forwardModelList.clear();
		for (auto& kv : m_reflectedModelList) {    // ���C���[���Ƃɒ��g��������
			kv.second.clear();
		}
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		// �ǂ̃��C���[�ɑ΂��Ă��A�Ή����郂�f�����X�g������� Execute
		for (auto it = m_planeReflectionPass.begin(); it != m_planeReflectionPass.end(); ++it) {
			ReflectLayer layer = it->first;
			auto& pass = it->second;
			auto& objects = m_reflectedModelList[layer];
			pass.UpdateReflectCamera(m_reflectPlane.at(layer));
			pass.Update();
			pass.Execute(rc, objects);
		}

		//GBuffer�ւ̃����_�����O
		RenderToGBuffer(rc);

		m_shadow.Execute(rc, m_deferredModelList);


		//�f�B�t�@�[�h���C�e�B���O
		DeferredLighting(rc);

		ForwardRendering(rc);

		//�P�x���o�ƃK�E�V�A���u���[���s
		//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
		PostEffecting(rc);

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[
		CopyMainRenderTargetToFrameBuffer(rc);

		//�`�悵���I�u�W�F�N�g���N���A����
		ObjectClear();


		//if (m_showReflectionDebug) {
		//	// �t���[���o�b�t�@��RTV/DSV�ɃZ�b�g
		//	g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
		//	// ��ʍ����Ȃǂɏo���iSprite�ɈʒuAPI���Ȃ���΃t���X�N���[���ł��j
		//	m_debugReflectionSprite.Draw(rc);
		//}
	}

	void RenderingEngine::PostEffecting(RenderContext& rc)
	{
		m_postEffect.Render(rc, m_mainRenderTarget);
	}

}