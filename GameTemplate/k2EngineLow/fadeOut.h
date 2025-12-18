#pragma once
#include "PostEffectBase.h"

namespace nsK2EngineLow {
	struct FadeCB
	{
		Vector3 collar;
		float pad;
		float fade;//現在のフェード量（0～1）
	};

	enum class FadeState
	{
		enFadeIN,
		enFadeOut,
		enFinishd
	};

	class FadeOut :public PostEffectBase
	{
	public:
		~FadeOut();

		void Init();

		void Update();

		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)override;

		void SetFadeCB(float fade, Vector3 collar) {
			m_fadeCB.collar = collar;
			m_fadeCB.fade = fade;
		}

		void StartFadeOut(float fadeTime, const Vector3& color);

		void StartFadeIn(float fadeTime, const Vector3& color);

		void StartFadeIn(float fadeTime);

	private:
		/// <summary>
		/// スプライトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitFinalSprite();

	private:
		Sprite m_finalSprite;
		FadeCB m_fadeCB;
		FadeState m_fadeState;
		float m_fadeTime;			//フェードに掛ける時間
		float m_elapsed;
		Vector3 m_collar;
	};
}
