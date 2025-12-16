
namespace nsK2EngineLow {
	struct FadeCB
	{

	};

	class FadeOut
	{
	public:
		void Init();

		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

		void SetFogCB(Vector3 collar, float fogDistanceScale) {
		}

	private:
		/// <summary>
		/// スプライトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitFinalSprite();

	private:
		Sprite m_finalSprite;
		FadeCB m_fadeCB;

	};
}
