#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{		//ブルームの初期化処理
		m_bloom.Init(mainRenderTarget);
	}
	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//ブルームの描画処理
		OnRenderBloom(rc, mainRenderTarget);

		//メインレンダーターゲットをPRESENTからRENDERTARGETへ。
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		EffectEngine::GetInstance()->Draw();
		//メインレンダーターゲットをTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	}
	void PostEffect::OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		m_bloom.OnRender(rc, mainRenderTarget);
	}
}
