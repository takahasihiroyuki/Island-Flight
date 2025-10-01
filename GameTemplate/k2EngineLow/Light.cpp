#include "k2EngineLowPreCompile.h"
#include "Light.h"
namespace nsK2EngineLow {
	namespace
	{
		const Vector3 SCENELIGHT_DHIRECTIONLIGHT_COLOR = Vector3(0.5f, 0.5f, 0.5f);
		const Vector3 SCENELIGHT_DHIRECTIONLIGHT_DIRECTION = Vector3(1.0f, 0.0f, -1.0f);
		const Vector3 SCENELIGHT_AMBIENTLIGHT_COLOR = Vector3(0.8f, 0.8f, 0.8f);
		const Vector3 LIGHTCAMERA_POSITION = Vector3(100.0f, 2000.0f, 500.0f);
	}

	void SceneLight::Init()
	{

		// �f�B���N�V�������C�g�̐ݒ�
		SetDirectionLight(
			SCENELIGHT_DHIRECTIONLIGHT_DIRECTION,
			SCENELIGHT_DHIRECTIONLIGHT_COLOR
		);

		// �A���r�G���g���C�g�̐ݒ�
		SetAmbientLight(
			SCENELIGHT_AMBIENTLIGHT_COLOR
		);

		SetLightCamera(
			LIGHTCAMERA_POSITION,
			SCENELIGHT_DHIRECTIONLIGHT_DIRECTION
		);


		// ���C�g�r���[���e�s��̐ݒ�
		SetLightLVP(
			m_lightCamera.GetViewProjectionMatrix()
		);

	}

}
