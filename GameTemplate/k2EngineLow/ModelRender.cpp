#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	void ModelRender::Init(const char* filePath, AnimationClip* animationClips, int numAnimationCrips, EnModelUpAxis enModelUpAxis,bool isShadowReciever)
	{
		// �X�P���g�����������B
		InitSkeleton(filePath);
		// �A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationCrips, enModelUpAxis);


		// �f�B���N�V�������C�g�̏������B
		SetDirectionLight(Vector3(1.0f, -1.0f, -1.0f), Vector3(0.5f, 0.5f, 0.5f));

		// GBuffer�`��p�̃��f����������
		InitModelOnRenderGBuffer(filePath, enModelUpAxis, isShadowReciever);

		// �����������B
		m_isInit = true;
	}

	void ModelRender::InitSkyCubeModel(ModelInitData& initData)
	{
	}

	void ModelRender::InitModel(const char* filePath, EnModelUpAxis enModelUpAxis)
	{
		ModelInitData initData;
		//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		initData.m_tkmFilePath = filePath;
		//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		initData.m_fxFilePath = "Assets/shader/model.fx";
		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		initData.m_vsEntryPointFunc = "VSMain";

		//�V�F�[�_�[���Ƀ��C�g�̏���n�����߂̕ϐ���ݒ肷��B
		initData.m_expandConstantBuffer = &GetSceneLight().GetSceneLight();
		initData.m_expandConstantBufferSize = sizeof(GetSceneLight().GetSceneLight());

		if (m_animationClips != nullptr) {
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			initData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//�X�P���g�����w�肷��B
			initData.m_skeleton = &m_skeleton;
		}

		//���f���̏�������w�肷��B
		initData.m_modelUpAxis = enModelUpAxis;

		//�쐬�����������f�[�^�����ƂɃ��f��������������A
		m_model.Init(initData);
	}

	void ModelRender::Update()
	{
		//�X�P���g�����X�V�B
		if (m_skeleton.IsInited())
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//���f���̍X�V�B
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//�A�j���[�V������i�߂�B
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());

	}

	void ModelRender::Draw(RenderContext& rc)
	{
		g_renderingEngine->AddModelList(this);
	}


	void ModelRender::SetWorldMatrix(const Matrix& matrix)
	{
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{

	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//�X�P���g���̃f�[�^��ǂݍ��݁B
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;

		//�A�j���[�V�������ݒ肳��Ă���Ȃ�
		if (m_animationClips != nullptr) {
			//�A�j���[�V�����̏������B
			m_animation.Init(
				m_skeleton,				//�A�j���[�V�����𗬂����ރX�P���g���B
				m_animationClips,		//�A�j���[�V�����N���b�v�B
				numAnimationClips		//�A�j���[�V�����N���b�v�̐��B
			);
		}

	}

	void ModelRender::InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBuffer.fx";
		modelInitData.m_psEntryPointFunc = "PSMain";

		//�A�j���[�V����������Ȃ�VSSkinMain���w��B
		if (m_animationClips != nullptr)
		{
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else
		{
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}

		modelInitData.m_modelUpAxis = enModelUpAxis;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_SNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_renderToGBufferModel.Init(modelInitData);

	}
}


