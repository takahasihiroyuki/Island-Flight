#pragma once
namespace nsK2EngineLow {
	class Bloom
	{
	public:
		void Init(RenderTarget& mainRenderTarget);

		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		/// <summary>
		/// �P�x�E�o�p�̃����_�[�^�[�Q�b�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitRenderTarget(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �P�x���o�p�̃X�v���C�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitLuminanceSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �{�P�摜�����Z��������X�v���C�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitFinalSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �K�E�V�A���u���[�̏�����
		/// </summary>
		void InitGaussianBlur();

	private:
		RenderTarget m_luminanceRenderTarget;

		Sprite m_luminanceSprite;
		Sprite m_finalSprite;

		GaussianBlur m_gaussianBlur[4];

	};
}

