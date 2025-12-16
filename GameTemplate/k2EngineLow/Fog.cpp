#include "k2EngineLowPreCompile.h"
#include "Fog.h"

void nsK2EngineLow::Fog::Init()
{
	InitFinalSprite(g_renderingEngine->GetZprepassRenderTarget().GetRenderTargetTexture());
}

void nsK2EngineLow::Fog::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//最終的な合成先としてメインレンダーターゲットを設定
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);

	// ブラー済みの輝度を合成する最終スプライトを描画
	m_finalSprite.Draw(rc);

	// 合成結果の描画完了を待機
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::Fog::InitFinalSprite(Texture& depthTexture)
{
	//初期化情報を設定する
	SpriteInitData finalSpriteInitData;

	finalSpriteInitData.m_textures[0] = &depthTexture;
	//mainRenderTargetとおなじ幅と高さ
	finalSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
	finalSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

	//ボケ画像を合成するので専用シェーダーを使用する
	finalSpriteInitData.m_fxFilePath = "Assets/shader/Fog.fx";
	finalSpriteInitData.m_psEntryPoinFunc = "PSMain";

	finalSpriteInitData.m_expandConstantBuffer = &m_fogCB;
	finalSpriteInitData.m_expandConstantBufferSize = sizeof(m_fogCB);


	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//ただし、加算合成で描画するので、アルファブレンディングモードを加算する
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

	//スプライトを初期化
	m_finalSprite.Init(finalSpriteInitData);

}
