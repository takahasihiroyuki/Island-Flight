#include "k2EngineLowPreCompile.h"
#include <algorithm>

nsK2EngineLow::FadeOut::~FadeOut()
{
}

void nsK2EngineLow::FadeOut::Init()
{
	InitFinalSprite();
}

void nsK2EngineLow::FadeOut::Update()
{
	if (m_fadeState == FadeState::enFinishd)return;

	m_elapsed += g_gameTime->GetFrameDeltaTime();

	float t = m_elapsed / m_fadeTime;

	if (t >= 1.0f) t = 1.0f;

	float fade = 0.0f;
	if (m_fadeState == FadeState::enFadeOut) {
		fade = t;           // 0～1
	}
	else if (m_fadeState == FadeState::enFadeIN) {
		fade = 1.0f - t;    // 1～0
	}

	SetFadeCB(fade, m_collar);

	if (t >= 1.0f) {
		m_fadeState = FadeState::enFinishd;
	}
}

void nsK2EngineLow::FadeOut::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	Update();

	//最終的な合成先としてメインレンダーターゲットを設定
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);

	m_finalSprite.Draw(rc);

	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::FadeOut::StartFadeOut(float fadeTime, const Vector3& collar)
{
	m_fadeState = FadeState::enFadeOut;

	m_fadeTime = fadeTime;
	if (m_fadeTime <= 0.0f)
	{
		m_fadeTime = 0.0001f;
	}
	m_elapsed = 0.0f;

	m_collar = collar;
	SetFadeCB(0.0f, m_collar);
}

void nsK2EngineLow::FadeOut::StartFadeIn(float fadeTime, const Vector3& collar)
{
	m_fadeState = FadeState::enFadeIN;
	m_fadeTime = fadeTime;
	if (m_fadeTime <= 0.0f)
	{
		m_fadeTime = 0.0001f;
	}
	m_elapsed = 0.0f;

	m_collar = collar;
	SetFadeCB(1.0f, m_collar);

}

void nsK2EngineLow::FadeOut::StartFadeIn(float fadeTime)
{
	m_fadeState = FadeState::enFadeIN;
	m_fadeTime = fadeTime;
	if (m_fadeTime <= 0.0f)
	{
		m_fadeTime = 0.0001f;
	}
	m_elapsed = 0.0f;

	SetFadeCB(1.0f, m_collar);
}

void nsK2EngineLow::FadeOut::InitFinalSprite()
{
	//初期化情報を設定する
	SpriteInitData finalSpriteInitData;

	finalSpriteInitData.m_ddsFilePath[0] = "Assets/sprite/white.dds";

	//mainRenderTargetとおなじ幅と高さ
	finalSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
	finalSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

	//ボケ画像を合成するので専用シェーダーを使用する
	finalSpriteInitData.m_fxFilePath = "Assets/shader/Fade.fx";
	finalSpriteInitData.m_psEntryPoinFunc = "PSMain";

	finalSpriteInitData.m_expandConstantBuffer = &m_fadeCB;
	finalSpriteInitData.m_expandConstantBufferSize = sizeof(m_fadeCB);


	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;

	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

	//スプライトを初期化
	m_finalSprite.Init(finalSpriteInitData);

}
