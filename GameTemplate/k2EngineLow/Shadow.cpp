#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace nsK2EngineLow
{
	void Shadow::Init()
	{
		// シャドウマップ用レンダリングターゲットの作成。
		shadowMapTarget.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

	}

	void Shadow::Execute(RenderContext& rc, std::vector<ModelRender*>& obj)
	{

		BeginGPUEvent("Shadow");

		UpdateLightCamera();

		//ターゲットをシャドウマップに変更
		rc.WaitUntilToPossibleSetRenderTarget(shadowMapTarget);
		rc.SetRenderTargetAndViewport(shadowMapTarget);
		rc.ClearRenderTargetView(shadowMapTarget);

		// まとめて影モデルレンダーを描画
		for (auto MobjData : obj)
		{
			////影モデルの描画
			//MobjData->OnRenderShadowMap(rc, g_renderingEngine->GetLightCamera());
		}

		rc.WaitUntilFinishDrawingToRenderTarget(shadowMapTarget);
	}
	void Shadow::UpdateLightCamera()
	{
		//
		Vector3 mainCameraPosition = g_camera3D->GetPosition();
		Vector3 lightDir = g_renderingEngine->GetSceneLight().GetDirLigDirection();
		lightDir.Normalize();

		Vector3 lightCameraPosition;
		lightCameraPosition = mainCameraPosition - lightDir * m_shadowDistance;
		g_renderingEngine->GetLightCamera().SetPosition(lightCameraPosition);
		g_renderingEngine->GetLightCamera().SetTarget(mainCameraPosition);
		g_renderingEngine->GetLightCamera().SetFar(m_shadowDistance*2000);
		g_renderingEngine->GetLightCamera().SetViewAngle(0.5);
	}
}

