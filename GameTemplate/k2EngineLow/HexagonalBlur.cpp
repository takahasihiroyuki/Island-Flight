#include "k2EngineLowPreCompile.h"
#include "HexagonalBlur.h"

void nsK2EngineLow::HexagonalBlur::Init(RenderTarget& mainRenderTarget)
{
	InitRenderTarget(mainRenderTarget);
	InitVertDiagonalBlurSprite(mainRenderTarget);
	InitRhomboidBlurSprite(mainRenderTarget);
	InitCombineBokeImageSprite(m_rtRhomboidBlur.GetRenderTargetTexture(), g_renderingEngine->GetZprepassRenderTarget().GetRenderTargetTexture());
}

void nsK2EngineLow::HexagonalBlur::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	ApplyVertDiagonalBlur(rc);
	ApplyHecagonalBlur(rc, mainRenderTarget);
}

void nsK2EngineLow::HexagonalBlur::InitRenderTarget(RenderTarget& mainRenderTarget)
{
	m_rtVerticalBlur.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_UNKNOWN);

	m_rtDiagonalBlur.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT
		, DXGI_FORMAT_UNKNOWN);

	m_rtRhomboidBlur.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_UNKNOWN);

}

void nsK2EngineLow::HexagonalBlur::InitVertDiagonalBlurSprite(RenderTarget& mainRenderTarget)
{
	SpriteInitData vertDiagonalBlurSpriteInitData;
	vertDiagonalBlurSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	vertDiagonalBlurSpriteInitData.m_width = 1280;
	vertDiagonalBlurSpriteInitData.m_height = 720;
	vertDiagonalBlurSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
	vertDiagonalBlurSpriteInitData.m_psEntryPoinFunc = "PSVerticalDiagonalBlur";
	vertDiagonalBlurSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	vertDiagonalBlurSpriteInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_vertDIagonalBlurSprite.Init(vertDiagonalBlurSpriteInitData);
}

void nsK2EngineLow::HexagonalBlur::InitRhomboidBlurSprite(RenderTarget& mainRenderTarget)
{
	SpriteInitData phomboidBlurSpriteInitData;
	phomboidBlurSpriteInitData.m_textures[0] = &m_rtVerticalBlur.GetRenderTargetTexture();
	phomboidBlurSpriteInitData.m_textures[1] = &m_rtDiagonalBlur.GetRenderTargetTexture();
	phomboidBlurSpriteInitData.m_width = 1280;
	phomboidBlurSpriteInitData.m_height = 720;
	phomboidBlurSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
	phomboidBlurSpriteInitData.m_psEntryPoinFunc = "PSRhomboidBlur";
	phomboidBlurSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_rhomboidBlurSprite.Init(phomboidBlurSpriteInitData);
}

void nsK2EngineLow::HexagonalBlur::InitCombineBokeImageSprite(Texture& bokeTexture, Texture& depthTexture)
{
	SpriteInitData combineBokeImageSpriteInitData;
	//使用するテクスチャは２枚。
	combineBokeImageSpriteInitData.m_textures[0] = &bokeTexture;
	combineBokeImageSpriteInitData.m_textures[1] = &depthTexture;
	//combineBokeImageSpriteInitData.m_expandShaderResoruceView[0] = &bokeTexture;
	//combineBokeImageSpriteInitData.m_expandShaderResoruceView[1] = &depthTexture;
	combineBokeImageSpriteInitData.m_width = 1280;
	combineBokeImageSpriteInitData.m_height = 720;
	combineBokeImageSpriteInitData.m_fxFilePath = "Assets/shader/BlendBokehByDepth.fx";
	combineBokeImageSpriteInitData.m_psEntryPoinFunc = "PSMain";
	combineBokeImageSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	// 距離を利用してボケ画像をアルファブレンディングするので、半透明合成モードにする。
	combineBokeImageSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
	// 初期化オブジェクトを利用してスプライトを初期化する。

	m_combineBokeImageSprite.Init(combineBokeImageSpriteInitData);

}

void nsK2EngineLow::HexagonalBlur::ApplyVertDiagonalBlur(RenderContext& rc)
{
	BeginGPUEvent("VertDiagonalBlur");

	RenderTarget* blurRts[] = {
	&m_rtVerticalBlur,
	&m_rtDiagonalBlur
	};

	// レンダリングターゲットとして利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTargets(2, blurRts);

	// レンダリングターゲットを設定
	rc.SetRenderTargetsAndViewport(2, blurRts);

	// レンダリングターゲットをクリア
	rc.ClearRenderTargetViews(2, blurRts);

	m_vertDIagonalBlurSprite.Draw(rc);

	// レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTargets(2, blurRts);
}

void nsK2EngineLow::HexagonalBlur::ApplyHecagonalBlur(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	BeginGPUEvent("HecagonalBlur");
	rc.WaitUntilToPossibleSetRenderTarget(m_rtRhomboidBlur);
	rc.SetRenderTargetAndViewport(m_rtRhomboidBlur);

	m_rhomboidBlurSprite.Draw(rc);

	// レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_rtRhomboidBlur);


	BeginGPUEvent("BlendBokehByDepth");
	// ボケ画像と深度テクスチャを利用して、ボケ画像を描きこんでいく
	// メインレンダリングターゲットを設定

	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);


	// スプライトを描画
	m_combineBokeImageSprite.Draw(rc);

	// レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);


}
