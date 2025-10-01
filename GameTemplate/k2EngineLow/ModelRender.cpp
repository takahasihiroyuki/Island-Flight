#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{
		for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); i++) {
			ReflectLayer layer = static_cast<ReflectLayer>(i);
			m_enableReflection[layer] = true;
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
		// スケルトンを初期化。
		InitSkeleton(filePath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationCrips, enModelUpAxis);

		//影を受ける側じゃないなら。
		if (!isShadowReciever) {
			// シャドウキャスター用のモデルを初期化。
			InitShadowModel(filePath, enModelUpAxis);
		}

		// GBuffer描画用のモデルを初期化
		InitModelOnRenderGBuffer(filePath, enModelUpAxis, isShadowReciever);

		//反射に移すなら。
		if (disableLayer == ReflectLayer::enNone) {
			InitReflectionModel(filePath, enModelUpAxis);
		}
		else {
			m_enableReflection[disableLayer] = false;
		}

		m_isFowardRender = isFowardRender;

		// 初期化完了。
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
	}

	void ModelRender::Update()
	{
		//モデル側に移動回転拡大を渡す
		m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//影のモデルに移動回転拡大を渡す
		m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//スケルトンを更新。
		if (m_skeleton.IsInited())
		{
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//モデルの更新。
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//アニメーションを進める。
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());

	}

	void ModelRender::Draw(RenderContext& rc)
	{
		if (!m_isFowardRender) {
			//ディファードレンダリングで描画するなら
			g_renderingEngine->AddDeferredModelList(this);
		}
		else {
			//フォワードレンダリングで描画するなら
			g_renderingEngine->AddForwardModelList(this);
		}

		//反射に映りこむなら
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
		if (m_ReflectionModel.IsInited())
		{
			m_ReflectionModel.Draw(rc, came, 1);
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
		//スケルトンのデータを読み込み。
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;

		//アニメーションが設定されているなら
		if (m_animationClips != nullptr) {
			//アニメーションの初期化。
			m_animation.Init(
				m_skeleton,				//アニメーションを流し込むスケルトン。
				m_animationClips,		//アニメーションクリップ。
				numAnimationClips		//アニメーションクリップの数。
			);
		}

	}

	void ModelRender::InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBuffer.fx";
		modelInitData.m_psEntryPointFunc = "PSMain";

		//アニメーションがあるならVSSkinMainを指定。
		if (m_animationClips != nullptr)
		{
			//スケルトンを指定する。
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

	void ModelRender::InitReflectionModel(const char* filePath, EnModelUpAxis enModelUpAxis)
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
		reflectionInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight().GetLight();
		reflectionInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight().GetLight());

		reflectionInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		reflectionInitData.m_modelUpAxis = enModelUpAxis;
		m_ReflectionModel.Init(reflectionInitData);
	}

}


