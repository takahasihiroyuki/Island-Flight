#pragma once
#include "PostEffectBase.h"

namespace nsK2EngineLow {
	struct FogCB
	{
		Vector3 fogCollar = Vector3{ 0.0f,0.0f,0.0f };
		float padding = 0.0f;
		float fogDistanceScale = 0.3f;//距離に対するフォグの強さ

	};

	class Fog :public PostEffectBase
	{
	public:
		void Init();

		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)override;

		void SetFogCB(Vector3 collar, float fogDistanceScale) {
			m_fogCB.fogCollar = collar;
			m_fogCB.fogDistanceScale = fogDistanceScale;
		}

	private:
		/// <summary>
		/// スプライトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitFinalSprite(Texture& depthTexture);

	private:
		Sprite m_finalSprite;
		FogCB m_fogCB;
	};
}

