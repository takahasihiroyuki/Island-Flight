#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

void nsK2EngineLow::Bloom::Init(RenderTarget& mainRenderTarget)
{
	InitRenderTarget(mainRenderTarget);
	InitLuminanceSprite(mainRenderTarget);
	InitGaussianBlur();
	InitFinalSprite(mainRenderTarget);
}

void nsK2EngineLow::Bloom::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//�@�P�x���o�p�����_�[�^�[�Q�b�g��ݒ肵�ĕ`�揀��
	rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
	rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
	rc.ClearRenderTargetView(m_luminanceRenderTarget);

	// �V�[������P�x�𒊏o����X�v���C�g��`��
	m_luminanceSprite.Draw(rc);

	// �P�x���o�̕`�抮����ҋ@
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

	//�P�x���ɑ΂��ăK�E�V�A���u���[�𕡐�����s
	m_gaussianBlur[0].ExecuteOnGPU(rc, 8);
	m_gaussianBlur[1].ExecuteOnGPU(rc, 8);
	m_gaussianBlur[2].ExecuteOnGPU(rc, 8);
	m_gaussianBlur[3].ExecuteOnGPU(rc, 8);

	//�ŏI�I�ȍ�����Ƃ��ă��C�������_�[�^�[�Q�b�g��ݒ�
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);

	// �u���[�ς݂̋P�x����������ŏI�X�v���C�g��`��
	m_finalSprite.Draw(rc);

	// �������ʂ̕`�抮����ҋ@
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::Bloom::InitRenderTarget(RenderTarget& mainRenderTarget)
{
	//�P�x���o�p�̃����_�����O�^�[�Q�b�g�B
	m_luminanceRenderTarget.Create(
		mainRenderTarget.GetWidth(),	          //���C�������_�����O�^�[�Q�b�g�Ɠ����B
		mainRenderTarget.GetHeight(),	          //���C�������_�����O�^�[�Q�b�g�Ɠ����B
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),  //���C�������_�����O�^�[�Q�b�g�Ɠ����B
		DXGI_FORMAT_D32_FLOAT
	);
}

void nsK2EngineLow::Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
{
	//���������쐬
	SpriteInitData luminanceSpriteInitData;
	//�P�x�E�o�p�̃V�F�[�_�[�̃t�@�C�����w�肷��
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
	//���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	//�X�v���C�g�̕��ƍ�����luminanceRenderTarget�Ɠ���
	luminanceSpriteInitData.m_width = m_luminanceRenderTarget.GetWidth();
	luminanceSpriteInitData.m_height = m_luminanceRenderTarget.GetHeight();

	//�e�N�X�`���̓��C�������_�[�^�[�Q�b�g�̃J���[�o�b�t�@�[
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//�������ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�쐬�����������������ɃX�v���C�g������������
	m_luminanceSprite.Init(luminanceSpriteInitData);
}

void nsK2EngineLow::Bloom::InitFinalSprite(RenderTarget& mainRenderTarget)
{
	//����������ݒ肷��
	SpriteInitData finalSpriteInitData;
	finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
	finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
	finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
	finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

	//mainRenderTarget�Ƃ��Ȃ����ƍ���
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();

	//�{�P�摜����������̂Ő�p�V�F�[�_�[���g�p����
	finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
	finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

	//�������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z����
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//�J���[�o�b�t�@�[�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�[
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//�X�v���C�g��������
	m_finalSprite.Init(finalSpriteInitData);

}

void nsK2EngineLow::Bloom::InitGaussianBlur()
{
	//[0]�͋P�x�e�N�X�`���ɃK�E�V�A���u���[��������
	m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
	//[0]�̃e�N�X�`���ɃK�E�V�A���u���[��������
	m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
	//[2]��[1]�̃e�N�X�`���ɃK�E�V�A���u���[��������
	m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
	//[3]��[2]�̃e�N�X�`���ɃK�E�V�A���u���[��������
	m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

}
