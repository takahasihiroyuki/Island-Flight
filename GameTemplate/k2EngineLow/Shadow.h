#pragma once

namespace nsK2EngineLow {

	class Shadow
	{
	public:
		void Init();

		void Execute(RenderContext& rc, std::vector<ModelRender*>& obj);

		RenderTarget& GetShadowTarget()
		{
			return shadowMapTarget;
		}
	private:
		RenderTarget shadowMapTarget;
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };	//�J���[�o�b�t�@�[�͐^����

	};
};
