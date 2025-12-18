#pragma once
namespace nsK2EngineLow {


	class PostEffectBase
	{
	public:

		virtual void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget) = 0;


		void SetEnabled(bool enable)
		{
			m_enable = enable;
		}

		bool GetEnabled() const
		{
			return m_enable;
		}
	private:
		bool m_enable;
	};
}

