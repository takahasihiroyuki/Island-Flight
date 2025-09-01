#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::SceneLight::Init()
{
	//ディレクションライトのカラー
	m_light.directionalLight.color.x = 0.5f;
	m_light.directionalLight.color.y = 0.5f;
	m_light.directionalLight.color.z = 0.5f;
	//ディレクションライトの方向
	m_light.directionalLight.lightDirection.x = 1.0f;
	m_light.directionalLight.lightDirection.y = -1.0f;
	m_light.directionalLight.lightDirection.z = -1.0f;
	m_light.directionalLight.lightDirection.Normalize();

	//カメラの座標
	m_light.cameraEyePos = g_camera3D->GetPosition();
}