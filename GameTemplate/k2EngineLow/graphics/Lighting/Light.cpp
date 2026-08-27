#include "k2EngineLowPreCompile.h"
#include "graphics/Lighting/Light.h"
namespace nsK2EngineLow {

	void SceneLight::Init(
		const Vector3& directionLightDir,
		const Vector3& directionLightColor,
		const Vector3& ambientLightColor,
		const Vector3& lightCameraPos,
		const float& lightCameraFar,
		const Matrix& mCameraViewProjInv
	)
	{

		// ディレクションライトの設定
		SetDirectionLight(
			directionLightDir,
			directionLightColor
		);

		// アンビエントライトの設定
		SetAmbientLight(
			ambientLightColor
		);

		// ライトカメラの設定
		SetLightCamera(
			lightCameraPos,
			directionLightDir,
			lightCameraFar
		);

		m_lightCamera.SetFar(100000000);
		m_lightCamera.Update();

		// ライトビュー投影行列の設定
		SetLightLVP(
			m_lightCamera.GetViewProjectionMatrix()
		);

		SetCameraViewProjInv(
			mCameraViewProjInv
		);

	}

}
