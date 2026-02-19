#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	namespace {
		Matrix CalcWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale, EnModelUpAxis modelUpAxis)
		{
			Matrix mBias, mWorld;
			if (modelUpAxis == enModelUpAxisY) {
				mBias.MakeRotationX(Math::PI * -0.5f);
			}

			Matrix mTrans, mRot, mScale;
			mTrans.MakeTranslation(pos);
			mRot.MakeRotationFromQuaternion(rot);
			mScale.MakeScaling(scale);
			mWorld = mBias * mScale * mRot * mTrans;
			return mWorld;
		}
	}
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
		size_t maxInstance,
		bool isFowardRender,
		ReflectLayer disableLayer
	)
	{
		// インスタンシング描画用のデータを初期化。
		InitInstancingDraw(maxInstance);
		// スケルトンを初期化。
		InitSkeleton(filePath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationCrips, enModelUpAxis);

		////影を受ける側じゃないなら。
		//if (!isShadowReciever) {
			// シャドウキャスター用のモデルを初期化。
			InitShadowModel(filePath, enModelUpAxis);
		//}

		// GBuffer描画用のモデルを初期化
		InitModelOnRenderGBuffer(filePath, enModelUpAxis, isShadowReciever);

		InitModelOnZprepass(filePath, enModelUpAxis);

		//反射で映り込まないレイヤーがあるなら。
		if (disableLayer != ReflectLayer::enNone) {
			// そのレイヤーの反射モデルをmapから削除。（forで邪魔になるので。）
			m_ReflectionModel.erase(disableLayer);
			m_enableReflection[disableLayer] = false;
		}

		//TODO:(auto it = m_enableReflection.begin(); it != m_enableReflection.end(); ++it)の間違い？
		for (auto it = m_ReflectionModel.begin(); it != m_ReflectionModel.end(); ++it) {
			auto& rayer = it->first;
			InitReflectionModel(filePath, enModelUpAxis, rayer);
		}

		m_isFowardRender = isFowardRender;

		m_modelUpAxis = enModelUpAxis;
		// 初期化完了。
		m_isInit = true;
	}

	void ModelRender::InitOcean(ModelInitData& initData, const char* tkmFilePath)
	{
		m_isFowardRender = true;
		m_enableReflection[ReflectLayer::enOcean] = false;

		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		m_frowardRenderModel.Init(initData);
		m_frowardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		InitModelOnZprepass(tkmFilePath, enModelUpAxisZ);
		InitInstancingDraw(1);

	}

	void ModelRender::InitSkyCubeModel(ModelInitData& initData, const char* tkmFilePath)
	{
		m_isFowardRender = true;
		m_frowardRenderModel.Init(initData);
		InitModelOnZprepass(tkmFilePath, enModelUpAxisZ,true);
		InitInstancingDraw(1);

	}

	void ModelRender::InitSkyCubeReflectionModel(ModelInitData& initData)
	{
		m_isFowardRender = true;
		for (auto it = m_ReflectionModel.begin(); it != m_ReflectionModel.end(); ++it) {
			auto& layer = it->first;
			m_enableReflection[layer] = true;
			m_ReflectionModel[layer].Init(initData);

		}
		InitInstancingDraw(1);
	}

	void ModelRender::Update()
	{
		if (m_isEnableInstancingDraw)return;

		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		if (m_isFowardRender)
		{
			if (m_frowardRenderModel.IsInited()) {
				//フォワードレンダリング用のモデルに移動回転拡大を渡す
				m_frowardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
			}
		}
		else {
			if (m_renderToGBufferModel.IsInited()) {
				//モデル側に移動回転拡大を渡す
				m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
			}
		}


		//影のモデルに移動回転拡大を渡す
		m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);


		for (auto it = m_ReflectionModel.begin(); it != m_ReflectionModel.end(); ++it) {
			auto& rayer = it->first;

			//反射のモデルに移動回転拡大を渡す
			m_ReflectionModel[rayer].UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}



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
		if (!m_visible)return;

		if (m_isEnableInstancingDraw) {
			m_worldMatrixArraySB.Update(m_worldMatrixArray.get());
		}

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

		//深度値用のモデルを登録
		//全てのモデルが登録する。
		g_renderingEngine->AddZprepassModelList(this);
	}

	void ModelRender::OnRenderShadowMap(RenderContext& rc, Camera& came)
	{
		if (m_shadowModel.IsInited())
		{
			m_shadowModel.Draw(rc, came, m_maxInstance);
		}

	}

	void ModelRender::OnRenderReflectionMap(RenderContext& rc, Camera& came)
	{
		for (auto it = m_ReflectionModel.begin(); it != m_ReflectionModel.end(); ++it) {
			auto& rayer = it->first;

			if (m_ReflectionModel[rayer].IsInited())
			{
				m_ReflectionModel[rayer].Draw(rc, came, m_maxInstance);
			}
		}
	}

	void ModelRender::SetWorldMatrix(const Matrix& matrix)
	{
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{

	}

	void ModelRender::UpdateInstancingData(
		int instanceNo,
		const char* instanceName,
		const Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale)
	{
		Matrix worldMatrix;
		worldMatrix = CalcWorldMatrix(pos, rot, scale, m_modelUpAxis);
		// インスタンス番号から行列のインデックスを取得する。
		int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];

		// ワールド行列の配列にワールド行列を設定する。
		m_worldMatrixArray[matrixArrayIndex] = worldMatrix;

	}

	void ModelRender::InitInstancingDraw(int maxInstance)
	{
		m_maxInstance = maxInstance;
		if (m_maxInstance > 1) {
			// ワールド行列の配列のメモリを確保する。
			m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);
			// ワールド行列をGPUに転送するためのストラクチャードバッファを確保。
			m_worldMatrixArraySB.Init(
				sizeof(Matrix),
				m_maxInstance,
				nullptr
			);
			m_isEnableInstancingDraw = true;
			// インスタンス番号からワールド行列の配列のインデックスに変換するテーブルを初期化する。
			m_instanceNoToWorldMatrixArrayIndexTable = std::make_unique<int[]>(m_maxInstance);
			for (int instanceNo = 0; instanceNo < m_maxInstance; instanceNo++) {
				m_instanceNoToWorldMatrixArrayIndexTable[instanceNo] = instanceNo;
			}
		}
		else {
			float debug = 0;
		}
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンのデータを読み込み。
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitModelOnZprepass(const char* tkmFilePath, EnModelUpAxis modelUpAxis, bool isSkyCube)
	{

		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		modelInitData.m_modelUpAxis = modelUpAxis;

		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
		modelInitData.m_vsEntryPointFunc = "VSMain";

		//アニメーションがあるならVSSkinMainを指定。
		if (m_animationClips != nullptr)
		{
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;

			if (m_isEnableInstancingDraw) {
				modelInitData.m_vsSkinEntryPointFunc = "VSSkinInstancingMain";
			}
			else {
				modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			}

		}
		else
		{
			if (m_isEnableInstancingDraw) {
				modelInitData.m_vsEntryPointFunc = "VSInstancingMain";
			}
			else {
				modelInitData.m_vsEntryPointFunc = "VSMain";
			}
		}

		if (isSkyCube)
		{
			modelInitData.m_psEntryPointFunc = "PSSkyCubeMain";
		}

		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		if (m_isEnableInstancingDraw) {
			// インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
			modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
		}

		m_zprepassModel.Init(modelInitData);

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

			if (m_isEnableInstancingDraw) {
				modelInitData.m_vsSkinEntryPointFunc = "VSSkinInstancingMain";
			}
			else {
				modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			}

		}
		else
		{
			if (m_isEnableInstancingDraw) {
				modelInitData.m_vsEntryPointFunc = "VSInstancingMain";
			}
			else {
				modelInitData.m_vsEntryPointFunc = "VSMain";
			}
		}

		if (isShadowReciever) {
			modelInitData.m_psEntryPointFunc = "PSMainShadowReciever";
		}
		else
		{
			modelInitData.m_psEntryPointFunc = "PSNormalMain";
		}
		if (m_isEnableInstancingDraw) {
			// インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
			modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
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
		if (m_isEnableInstancingDraw) {
			// インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
			shadowInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
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

		reflectionInitData.m_psEntryPointFunc = "PSMain";

		if (m_animationClips != nullptr) {
			if (m_isEnableInstancingDraw) {
				reflectionInitData.m_vsSkinEntryPointFunc = "VSSkinInstancingMain";
			}
			else {
				reflectionInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			}
			reflectionInitData.m_skeleton = &m_skeleton;
		}
		else
		{
			if (m_isEnableInstancingDraw) {
				reflectionInitData.m_vsEntryPointFunc = "VSInstancingMain";
			}
			else {
				reflectionInitData.m_vsEntryPointFunc = "VSMain";
			}
		}
		reflectionInitData.m_expandConstantBuffer = &g_renderingEngine->GetReflectionModelCB(layer);
		reflectionInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetReflectionModelCB(layer));
		if (m_isEnableInstancingDraw) {
			// インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
			reflectionInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
		}

		reflectionInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		reflectionInitData.m_modelUpAxis = enModelUpAxis;
		m_ReflectionModel[layer].Init(reflectionInitData);
	}

	void ModelRender::RemoveInstance(int instanceNo)
	{
		int matrixIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];

		m_worldMatrixArray[matrixIndex] = Matrix::Identity;
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


