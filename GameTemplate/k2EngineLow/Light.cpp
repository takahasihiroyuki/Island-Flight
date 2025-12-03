#include "k2EngineLowPreCompile.h"
#include "Light.h"
namespace nsK2EngineLow {
	namespace
	{
		const Vector3 SCENELIGHT_DHIRECTIONLIGHT_COLOR = Vector3(1.0f, 0.95f, 0.80f)*0.5;
		const Vector3 SCENELIGHT_DHIRECTIONLIGHT_DIRECTION = Vector3(0.8f, 0.1f, 0.1f);
		const Vector3 SCENELIGHT_AMBIENTLIGHT_COLOR = Vector3(0.43f, 0.47f, 0.50f)*2;
		const Vector3 LIGHTCAMERA_POSITION = Vector3(100.0f, 2000.0f, 500.0f);
	}

	void SceneLight::Init()
	{

		// ディレクションライトの設定
		SetDirectionLight(
			SCENELIGHT_DHIRECTIONLIGHT_DIRECTION,
			SCENELIGHT_DHIRECTIONLIGHT_COLOR
		);

		// アンビエントライトの設定
		SetAmbientLight(
			SCENELIGHT_AMBIENTLIGHT_COLOR
		);

		SetLightCamera(
			LIGHTCAMERA_POSITION,
			SCENELIGHT_DHIRECTIONLIGHT_DIRECTION
		);


		// ライトビュー投影行列の設定
		SetLightLVP(
			m_lightCamera.GetViewProjectionMatrix()
		);

	}

}
