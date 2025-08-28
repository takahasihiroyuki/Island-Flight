#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

void nsK2EngineLow::ModelRender::Init(const char* filePath, AnimationClip* animationClips, int numAnimationCrips, EnModelUpAxis enModelUpAxis)
{
	// スケルトンを初期化。
	InitSkeleton(filePath);
	// アニメーションを初期化。
	InitAnimation(animationClips, numAnimationCrips, enModelUpAxis);

	InitModel(filePath, enModelUpAxis);

	// 初期化完了。
	m_isInit = true;
}

void nsK2EngineLow::ModelRender::InitSkyCubeModel(ModelInitData& initData)
{
}

void nsK2EngineLow::ModelRender::InitModel(const char* filePath, EnModelUpAxis enModelUpAxis)
{
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = filePath;
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
	initData.m_vsEntryPointFunc = "VSMain";


	if (m_animationClips != nullptr) {
		//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
		initData.m_vsSkinEntryPointFunc = "VSSkinMain";
		//スケルトンを指定する。
		initData.m_skeleton = &m_skeleton;
	}

	//モデルの上方向を指定する。
	initData.m_modelUpAxis = enModelUpAxis;

	//作成した初期化データをもとにモデルを初期化する、
	m_model.Init(initData);
}

void nsK2EngineLow::ModelRender::Update()
{
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

void nsK2EngineLow::ModelRender::Draw(RenderContext& rc)
{
	m_model.Draw(rc);
}

void nsK2EngineLow::ModelRender::SetWorldMatrix(const Matrix& matrix)
{
}

void nsK2EngineLow::ModelRender::UpdateWorldMatrixInModes()
{

}

void nsK2EngineLow::ModelRender::InitSkeleton(const char* filePath)
{
	//スケルトンのデータを読み込み。
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");
	m_skeleton.Init(skeletonFilePath.c_str());
}

void nsK2EngineLow::ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
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
