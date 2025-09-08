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
	//　輝度抽出用レンダーターゲットを設定して描画準備
	rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
	rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
	rc.ClearRenderTargetView(m_luminanceRenderTarget);

	// シーンから輝度を抽出するスプライトを描画
	m_luminanceSprite.Draw(rc);

	// 輝度抽出の描画完了を待機
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

	//輝度情報に対してガウシアンブラーを複数回実行
	m_gaussianBlur[0].ExecuteOnGPU(rc, 8);
	m_gaussianBlur[1].ExecuteOnGPU(rc, 8);
	m_gaussianBlur[2].ExecuteOnGPU(rc, 8);
	m_gaussianBlur[3].ExecuteOnGPU(rc, 8);

	//最終的な合成先としてメインレンダーターゲットを設定
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);

	// ブラー済みの輝度を合成する最終スプライトを描画
	m_finalSprite.Draw(rc);

	// 合成結果の描画完了を待機
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::Bloom::InitRenderTarget(RenderTarget& mainRenderTarget)
{
	//輝度抽出用のレンダリングターゲット。
	m_luminanceRenderTarget.Create(
		mainRenderTarget.GetWidth(),	          //メインレンダリングターゲットと同じ。
		mainRenderTarget.GetHeight(),	          //メインレンダリングターゲットと同じ。
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),  //メインレンダリングターゲットと同じ。
		DXGI_FORMAT_D32_FLOAT
	);
}

void nsK2EngineLow::Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
{
	//初期化情報作成
	SpriteInitData luminanceSpriteInitData;
	//輝度摘出用のシェーダーのファイルを指定する
	luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
	//頂点シェーダーのエントリーポイントを指定する
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//ピクセルシェーダーのエントリーポイントを指定する
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	//スプライトの幅と高さはluminanceRenderTargetと同じ
	luminanceSpriteInitData.m_width = m_luminanceRenderTarget.GetWidth();
	luminanceSpriteInitData.m_height = m_luminanceRenderTarget.GetHeight();

	//テクスチャはメインレンダーターゲットのカラーバッファー
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//書き込むレンダリングターゲットのフォーマットを指定する
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//作成した初期化情報を元にスプライトを初期化する
	m_luminanceSprite.Init(luminanceSpriteInitData);
}

void nsK2EngineLow::Bloom::InitFinalSprite(RenderTarget& mainRenderTarget)
{
	//初期化情報を設定する
	SpriteInitData finalSpriteInitData;
	finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
	finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
	finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
	finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

	//mainRenderTargetとおなじ幅と高さ
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();

	//ボケ画像を合成するので専用シェーダーを使用する
	finalSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
	finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

	//ただし、加算合成で描画するので、アルファブレンディングモードを加算する
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//カラーバッファーのフォーマットは例によって、32ビット浮動小数点バッファー
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//スプライトを初期化
	m_finalSprite.Init(finalSpriteInitData);

}

void nsK2EngineLow::Bloom::InitGaussianBlur()
{
	//[0]は輝度テクスチャにガウシアンブラーをかける
	m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
	//[0]のテクスチャにガウシアンブラーをかける
	m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
	//[2]は[1]のテクスチャにガウシアンブラーをかける
	m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
	//[3]は[2]のテクスチャにガウシアンブラーをかける
	m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

}
