#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"
#include "FadeOut.h"

namespace nsK2EngineLow {
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{
		//ブルームの初期化処理
		m_bloom.Init(mainRenderTarget);
		m_hexagonalBlur.Init(mainRenderTarget);
		m_fog.Init();
		m_fade.Init();
	}
	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		EffectEngine::GetInstance()->Draw();

		//ブルームの描画処理
		OnRenderBloom(rc, mainRenderTarget);

		//フォグの描画
		OnRenderFog(rc, mainRenderTarget);

		//六角形のブラーの描画
		OnRenderHexagonBlur(rc, mainRenderTarget);

		//メインレンダーターゲットをPRESENTからRENDERTARGETへ。
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);

		//メインレンダーターゲットをTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	}
	void PostEffect::OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		if (m_bloom.GetEnabled())
		{
			m_bloom.OnRender(rc, mainRenderTarget);
		}
	}
	void PostEffect::OnRenderHexagonBlur(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		if (m_hexagonalBlur.GetEnabled())
		{
			m_hexagonalBlur.OnRender(rc, mainRenderTarget);
		}
	}
	void PostEffect::OnRenderFog(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		if (m_fog.GetEnabled())
		{
			m_fog.OnRender(rc, mainRenderTarget);
		}
	}
	void PostEffect::OnRenderFade(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		if (m_fade.GetEnabled())
		{
			m_fade.OnRender(rc, mainRenderTarget);
		}
	}
}
