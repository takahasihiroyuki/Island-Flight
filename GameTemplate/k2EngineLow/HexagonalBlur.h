#pragma once
namespace nsK2EngineLow {

	class HexagonalBlur
	{
	public:
		void Init(RenderTarget& mainRenderTarget);

		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		void InitRenderTarget(RenderTarget& mainRenderTarget);

		/// <summary>
		/// c•ûŒü‚Æ‘ÎŠpü‚Ìƒuƒ‰[‚ğŠ|‚¯‚é‚½‚ß‚ÌƒXƒvƒ‰ƒCƒg‚ğ‰Šú‰»‚·‚é
		/// </summary>
		void InitVertDiagonalBlurSprite(RenderTarget& mainRenderTarget);

		/// <summary>
		/// ˜ZŠpŒ`ƒuƒ‰[‚ğŠ|‚¯‚é‚½‚ß‚ÌƒXƒvƒ‰ƒCƒg‚ğ‰Šú‰»
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void InitRhomboidBlurSprite(RenderTarget& mainRenderTarget);

		void InitCombineBokeImageSprite(Texture& bokeTexture, Texture& depthTexture);
		/// <summary>
		/// c‚Æ‘ÎŠpü‚Ìƒuƒ‰[‚ğŠ|‚¯‚é
		/// </summary>
		/// <param name="rc"></param>
		void ApplyVertDiagonalBlur(RenderContext& rc);

		/// <summary>
		/// ˜ZŠpŒ`ƒuƒ‰[‚ğŠ|‚¯‚é
		/// </summary>
		/// <param name="rc"></param>
		void ApplyHecagonalBlur(RenderContext& rc, RenderTarget& mainRenderTarget);


	private:
		RenderTarget m_rtVerticalBlur;
		RenderTarget m_rtDiagonalBlur;
		RenderTarget m_rtRhomboidBlur;

		Sprite m_vertDIagonalBlurSprite;
		Sprite m_rhomboidBlurSprite;
		Sprite m_combineBokeImageSprite;

	};

}