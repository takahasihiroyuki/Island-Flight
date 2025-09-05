#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::SceneLight::Init()
{
	//�f�B���N�V�������C�g�̃J���[
	m_light.directionalLight.color.x = 1.0f;
	m_light.directionalLight.color.y = 1.0f;
	m_light.directionalLight.color.z = 1.0f;
	//�f�B���N�V�������C�g�̕���
	m_light.directionalLight.lightDirection.x = 1.0f;
	m_light.directionalLight.lightDirection.y = -1.0f;
	m_light.directionalLight.lightDirection.z = -1.0f;
	m_light.directionalLight.lightDirection.Normalize();

	//�J�����̍��W
	m_light.cameraEyePos = g_camera3D->GetPosition();
}