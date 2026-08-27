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
		float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };	//カラーバッファーは真っ黒
		Camera m_reflectCamera;                         // 反射用カメラ
	};
}