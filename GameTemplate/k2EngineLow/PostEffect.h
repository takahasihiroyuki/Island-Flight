#pragma once
#include "Bloom.h"


namespace nsK2EngineLow {

	class PostEffect
	{
	public:
		/// <summary>
		/// �|�X�g�G�t�F�N�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void Init(
			RenderTarget& mainRenderTarget
		);

		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void Render(
			RenderContext& rc,
			RenderTarget& mainRenderTarget
		);

		/// <summary>
		/// �u���[��
		/// </summary>
		/// <param name="rc"><�����_�[�R���e�L�X�g/param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		Bloom m_bloom;		//�u���[��
	};
}
