#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{		//�u���[���̏���������
		m_bloom.Init(mainRenderTarget);
	}
	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//�u���[���̕`�揈��
		OnRenderBloom(rc, mainRenderTarget);

		//���C�������_�[�^�[�Q�b�g��PRESENT����RENDERTARGET�ցB
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		EffectEngine::GetInstance()->Draw();
		//���C�������_�[�^�[�Q�b�g��TARGET����PRESENT�ցB
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	}
	void PostEffect::OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		m_bloom.OnRender(rc, mainRenderTarget);
	}
}
