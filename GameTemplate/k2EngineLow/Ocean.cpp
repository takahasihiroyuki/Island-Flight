#include "k2EngineLowPreCompile.h"
namespace nsK2EngineLow {
	Ocean::Ocean()
	{
	}
	Ocean::~Ocean()
	{
	}
	bool Ocean::Start()
	{
		SetConstatntBuffer(
			g_renderingEngine->GetReflectViewMatrix(ReflectLayer::enOcean),
			g_renderingEngine->GetSceneLight().GetLight(),
			g_renderingEngine->GetReflectCamera(ReflectLayer::enOcean).GetPosition()
		);

		g_renderingEngine->SetReflectPlane(m_plane, ReflectLayer::enOcean);

		ModelInitData initData;
		//tkmファイルのファイルパスを指定する。
		initData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
		//シェーダーファイルのファイルパスを指定する。
		initData.m_fxFilePath = "Assets/shader/Ocean.fx";
		initData.m_vsEntryPointFunc = "VSMain";
		initData.m_psEntryPointFunc = "PSMain";

		m_reflectionRenderTarget = &g_renderingEngine->GetPlaneReflectionRenderTarget(ReflectLayer::enOcean);
		initData.m_expandShaderResoruceView[0] = &m_reflectionRenderTarget->GetRenderTargetTexture();

		initData.m_expandConstantBuffer = &m_constantBuffer;
		initData.m_expandConstantBufferSize = sizeof(m_constantBuffer);


		m_modelRender.InitOcean(initData);
		m_modelRender.SetTransform(m_position, g_quatIdentity, m_scale);
		//m_modelRender.ChangeAlbedoMap("", m_reflectionRenderTarget->GetRenderTargetTexture());
		m_modelRender.Update();
		return true;
	}
	void Ocean::Init()
	{
	}
	//void Ocean::InitOceanSprite()
	//{
	//	SpriteInitData spriteInitData;
	//	spriteInitData.m_width = FRAME_BUFFER_W;
	//	spriteInitData.m_height = FRAME_BUFFER_H;
	//	spriteInitData.m_textures[0] = &m_reflectionRenderTarget->GetRenderTargetTexture();
	//}
	void Ocean::Update()
	{
		SetConstatntBuffer(
			g_renderingEngine->GetReflectViewMatrix(ReflectLayer::enOcean),
			g_renderingEngine->GetSceneLight().GetLight(),
			g_renderingEngine->GetReflectCamera(ReflectLayer::enOcean).GetPosition()
		);
	}
	void Ocean::Render(RenderContext& rc)
	{
		m_modelRender.Draw(rc);
	}

}
