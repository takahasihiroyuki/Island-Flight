#include "k2EngineLowPreCompile.h"
#include "FadeOut.h"

void nsK2EngineLow::FadeOut::Init()
{
	InitFinalSprite();
}

void nsK2EngineLow::FadeOut::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//最終的な合成先としてメインレンダーターゲットを設定
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);

	// ブラー済みの輝度を合成する最終スプライトを描画
	m_finalSprite.Draw(rc);

	// 合成結果の描画完了を待機
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::FadeOut::InitFinalSprite()
{
	//初期化情報を設定する
	SpriteInitData finalSpriteInitData;

	//mainRenderTargetとおなじ幅と高さ
	finalSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
	finalSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

	//ボケ画像を合成するので専用シェーダーを使用する
	finalSpriteInitData.m_fxFilePath = "Assets/shader/Fade.fx";
	finalSpriteInitData.m_psEntryPoinFunc = "PSMain";

	finalSpriteInitData.m_expandConstantBuffer = &m_fadeCB;
	finalSpriteInitData.m_expandConstantBufferSize = sizeof(m_fadeCB);


	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;

	//ただし、加算合成で描画するので、アルファブレンディングモードを加算する
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

	//スプライトを初期化
	m_finalSprite.Init(finalSpriteInitData);

}
