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
		/// 縦方向と対角線のブラーを掛けるためのスプライトを初期化する
		/// </summary>
		void InitVertDiagonalBlurSprite(RenderTarget& mainRenderTarget);

		/// <summary>
		/// 六角形ブラーを掛けるためのスプライトを初期化
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void InitRhomboidBlurSprite(RenderTarget& mainRenderTarget);

		void InitCombineBokeImageSprite(Texture& bokeTexture, Texture& depthTexture);
		/// <summary>
		/// 縦と対角線のブラーを掛ける
		/// </summary>
		/// <param name="rc"></param>
		void ApplyVertDiagonalBlur(RenderContext& rc);

		/// <summary>
		/// 六角形ブラーを掛ける
		/// </summary>
		/// <param name="rc"></param>
		void ApplyHecagonalBlur(RenderContext& rc);

		/// <summary>
		/// 六角形ブラーを掛けた画像を深度とブレンドする
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void BlendHexBlurWithDepth(RenderContext& rc, RenderTarget& mainRenderTarget);


	private:
		RenderTarget m_rtVerticalBlur;
		RenderTarget m_rtDiagonalBlur;
		RenderTarget m_rtRhomboidBlur;

		Sprite m_vertDIagonalBlurSprite;
		Sprite m_rhomboidBlurSprite;
		Sprite m_combineBokeImageSprite;

	};

}