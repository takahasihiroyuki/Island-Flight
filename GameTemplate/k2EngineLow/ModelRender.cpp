#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

void nsK2EngineLow::ModelRender::Init(const char* filePath, AnimationClip* animationClips, int numAnimationCrips, EnModelUpAxis enModelUpAxis)
{
	// �X�P���g�����������B
	InitSkeleton(filePath);
	// �A�j���[�V�������������B
	InitAnimation(animationClips, numAnimationCrips, enModelUpAxis);

	InitModel(filePath, enModelUpAxis);

	// �����������B
	m_isInit = true;
}

void nsK2EngineLow::ModelRender::InitSkyCubeModel(ModelInitData& initData)
{
}

void nsK2EngineLow::ModelRender::InitModel(const char* filePath, EnModelUpAxis enModelUpAxis)
{
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = filePath;
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	initData.m_vsEntryPointFunc = "VSMain";


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

void nsK2EngineLow::ModelRender::Update()
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
	//�X�P���g���̃f�[�^��ǂݍ��݁B
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");
	m_skeleton.Init(skeletonFilePath.c_str());
}

void nsK2EngineLow::ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
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
