#pragma once
namespace nsK2EngineLow
{
	class PlanarReflectionPass
	{
	public:
		PlanarReflectionPass();
		~PlanarReflectionPass();
		void Init();
		void Execute(RenderContext& rc);
		void UpdateReflectCamera();


	private:
		RenderTarget planarReflectionTarget;
		float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };	//�J���[�o�b�t�@�[�͐^����
		Camera m_reflectCamera;                         // ���˗p�J����
	};
}