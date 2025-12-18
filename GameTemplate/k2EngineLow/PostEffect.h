#pragma once
#include "Bloom.h"
#include "HexagonalBlur.h"
#include "Fog.h"
#include"FadeOut.h"


namespace nsK2EngineLow {

	class PostEffect
	{
	public:
		/// <summary>
		/// ポストエフェクトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Init(
			RenderTarget& mainRenderTarget
		);

		void StartFadeIn(float time) {
			m_fade.StartFadeIn(time);           // 現在メンバの色を使う
		}

		void StartFadeIn(float fadeTime, const Vector3& collar) {
			m_fade.StartFadeIn(fadeTime, collar);
		}

		void StartFadeOut(float fadeTime, const Vector3& collar) {
			m_fade.StartFadeOut(fadeTime, collar);
		}

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Render(
			RenderContext& rc,
			RenderTarget& mainRenderTarget
		);

		/// <summary>
		/// ブルーム
		/// </summary>
		/// <param name="rc"><レンダーコンテキスト/param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget);

		void OnRenderHexagonBlur(RenderContext& rc, RenderTarget& mainRenderTarget);

		void OnRenderFog(RenderContext& rc, RenderTarget& mainRenderTarget);

		void OnRenderFade(RenderContext& rc, RenderTarget& mainRenderTarget);

		//フォグのパラメータを設定。
		void SetFogParams(Vector3 collar, float fogDistanceScale) {
			m_fog.SetFogCB(collar, fogDistanceScale);
		}

		void SetFadeEnabled(bool enable)
		{
			m_fade.SetEnabled(enable);
		}

		void SetHexagonalBlurEnabled(bool enable)
		{
			m_hexagonalBlur.SetEnabled(enable);
		}

		void SetBloomEnabled(bool enable)
		{
			m_bloom.SetEnabled(enable);
		}

		void SetFogEnabled(bool enable)
		{
			m_fog.SetEnabled(enable);
		}

	private:
		Bloom m_bloom;		//ブルーム
		HexagonalBlur m_hexagonalBlur;
		Fog m_fog;
		FadeOut m_fade;
	};
}
