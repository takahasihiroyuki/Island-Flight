#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{
		for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); i++) {
			ReflectLayer layer = static_cast<ReflectLayer>(i);
			m_enableReflection[layer] = true;
			m_ReflectionModel.try_emplace(layer);
		}
	}
	ModelRender::~ModelRender()
	{
	}
	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationClips,
		int numAnimationCrips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReciever,
		bool isFowardRender,
		ReflectLayer disableLayer
	)
	{
		// �X�P���g�����������B
		InitSkeleton(filePath);
		// �A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationCrips, enModelUpAxis);

		//�e���󂯂鑤����Ȃ��Ȃ�B
		if (!isShadowReciever) {
			// �V���h�E�L���X�^�[�p�̃��f�����������B
			InitShadowModel(filePath, enModelUpAxis);
		}

		// GBuffer�`��p�̃��f����������
		InitModelOnRenderGBuffer(filePath, enModelUpAxis, isShadowReciever);

		//���˂ŉf�荞�܂Ȃ����C���[������Ȃ�B
		if (disableLayer != ReflectLayer::enNone) {
			// ���̃��C���[�̔��˃��f����map����폜�B�ifor�Ŏז��ɂȂ�̂ŁB�j
			m_ReflectionModel.erase(disableLayer);
			m_enableReflection[disableLayer] = false;
		}


		for (auto it = m_ReflectionModel.begin(); it != m_ReflectionModel.end(); ++it) {
			auto& rayer = it->first;
			InitReflectionModel(filePath, enModelUpAxis, rayer);
		}

		m_isFowardRender = isFowardRender;

		m_model;
		// �����������B
		m_isInit = true;
	}

	void ModelRender::InitOcean(ModelInitData& initData)
	{
		m_isFowardRender = true;
		m_enableReflection[ReflectLayer::enOcean] = false;

		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		m_frowardRenderModel.Init(initData);
		m_frowardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

	void ModelRender::InitSkyCubeModel(ModelInitData& initData)
	{
		m_isFowardRender = true;
		m_frowardRenderModel.Init(initData);

	}

	void ModelRender::InitSkyCubeReflectionModel(ModelInitData& initData)
	{
		m_isFowardRender = true;
		for (auto it = m_ReflectionModel.begin(); it != m_ReflectionModel.end(); ++it) {
			auto& layer = it->first;
			m_enableReflection[layer] = true;
			m_ReflectionModel[layer].Init(initData);

		}
	}

	void ModelRender::Update()
	{
		if (m_isFowardRender)
		{
			if (m_frowardRenderModel.IsInited()) {
				//�t�H���[�h�����_�����O�p�̃��f���Ɉړ���]�g���n��
				m_frowardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
			}
		}
		else {
			if (m_renderToGBufferModel.IsInited()) {
				//���f�����Ɉړ���]�g���n��
				m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
			}
		}


		//�e�̃��f���Ɉړ���]�g���n��
		m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		for (auto it = m_ReflectionModel.begin(); it != m_ReflectionModel.end(); ++it) {
			auto& rayer = it->first;

			//���˂̃��f���Ɉړ���]�g���n��
			m_ReflectionModel[rayer].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}



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
		if (!m_isFowardRender) {
			//�f�B�t�@�[�h�����_�����O�ŕ`�悷��Ȃ�
			g_renderingEngine->AddDeferredModelList(this);
		}
		else {
			//�t�H���[�h�����_�����O�ŕ`�悷��Ȃ�
			g_renderingEngine->AddForwardModelList(this);
		}

		//���˂ɉf�肱�ނȂ�
		for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); i++) {
			ReflectLayer layer = static_cast<ReflectLayer>(i);
			if (m_enableReflection[layer]) {
				g_renderingEngine->AddreflectedModelList(this, layer);
			}
		}
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc, Camera& came)
	{
		if (m_shadowModel.IsInited())
		{
			m_shadowModel.Draw(rc, came, 1);
		}

	}

	void ModelRender::OnRenderReflectionMap(RenderContext& rc, Camera& came)
	{
		for (auto it = m_ReflectionModel.begin(); it != m_ReflectionModel.end(); ++it) {
			auto& rayer = it->first;

			if (m_ReflectionModel[rayer].IsInited())
			{
				m_ReflectionModel[rayer].Draw(rc, came, 1);
			}
		}
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
		if (isShadowReciever) {
			modelInitData.m_psEntryPointFunc = "PSMainShadowReciever";
		}
		else
		{
			modelInitData.m_psEntryPointFunc = "PSNormalMain";
		}
		modelInitData.m_modelUpAxis = enModelUpAxis;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_SNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_renderToGBufferModel.Init(modelInitData);

	}
	void ModelRender::InitShadowModel(const char* filePath, EnModelUpAxis enModelUpAxis)
	{
		ModelInitData shadowInitData;
		shadowInitData.m_tkmFilePath = filePath;
		shadowInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		shadowInitData.m_vsEntryPointFunc = "VSMain";
		shadowInitData.m_psEntryPointFunc = "PSShadowCaster";
		if (m_animationClips != nullptr) {
			shadowInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			shadowInitData.m_skeleton = &m_skeleton;
		}
		shadowInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

		shadowInitData.m_modelUpAxis = enModelUpAxis;
		m_shadowModel.Init(shadowInitData);
	}

	void ModelRender::InitReflectionModel(const char* filePath, EnModelUpAxis enModelUpAxis, ReflectLayer layer)
	{
		ModelInitData reflectionInitData;
		reflectionInitData.m_tkmFilePath = filePath;
		reflectionInitData.m_fxFilePath = "Assets/shader/DrawReflection.fx";
		reflectionInitData.m_vsEntryPointFunc = "VSMain";

			reflectionInitData.m_psEntryPointFunc = "PSMain";

		if (m_animationClips != nullptr) {
			reflectionInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			reflectionInitData.m_skeleton = &m_skeleton;
		}
		reflectionInitData.m_expandConstantBuffer = &g_renderingEngine->GetReflectionModelCB(layer);
		reflectionInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetReflectionModelCB(layer));

		reflectionInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		reflectionInitData.m_modelUpAxis = enModelUpAxis;
		m_ReflectionModel[layer].Init(reflectionInitData);
	}

	//void ModelRender::InitSkyCubeReflectionModel(const char* filePath, EnModelUpAxis enModelUpAxis, ReflectLayer layer)
	//{
	//	ModelInitData reflectionInitData;
	//	reflectionInitData.m_tkmFilePath = filePath;
	//	reflectionInitData.m_fxFilePath = "Assets/shader/SkyCubeReflection.fx";
	//	reflectionInitData.m_vsEntryPointFunc = "VSMain";

	//	reflectionInitData.m_psEntryPointFunc = "PSMain";

	//	reflectionInitData.m_expandConstantBuffer = &g_renderingEngine->GetReflectionModelCB(layer);
	//	reflectionInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetReflectionModelCB(layer));

	//	reflectionInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	//	reflectionInitData.m_modelUpAxis = enModelUpAxis;
	//	m_ReflectionModel[layer].Init(reflectionInitData);
	//}

}


