#include "k2EngineLowPreCompile.h"
#include "Light.h"
namespace nsK2EngineLow {

	void SceneLight::Init(
		const Vector3& directionLightDir,
		const Vector3& directionLightColor,
		const Vector3& ambientLightColor,
		const Vector3& lightCameraPos,
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
            directionLightDir
        );

        // ライトビュー投影行列の設定
        SetLightLVP(
            m_lightCamera.GetViewProjectionMatrix()
        );

        SetCameraViewProjInv(
            mCameraViewProjInv
        );

	}

}
