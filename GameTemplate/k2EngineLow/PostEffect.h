#pragma once
#include "Bloom.h"


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

	private:
		Bloom m_bloom;		//ブルーム
	};
}
