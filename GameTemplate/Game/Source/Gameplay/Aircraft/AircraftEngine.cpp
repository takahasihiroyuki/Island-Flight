#include "Source/stdafx.h"
#include "Source/Gameplay/Aircraft/AircraftEngine.h"
namespace {
	constexpr float MAX_THROTLEINPUT = 1.0f;
}

void AircraftEngine::UpdateThrustForce()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();
	float thrustScale = 0;//推力の大きさ

	//エンジンがかかっているときにスロットルを上げる。
	if (!m_isThrottleCut)
		m_throttleSmoothValue += deltaTime;
	else
		m_throttleSmoothValue -= deltaTime;

	if (m_throttleSmoothValue <= 0)m_throttleSmoothValue = 0;
	if (m_throttleSmoothValue >= MAX_THROTLEINPUT)m_throttleSmoothValue = MAX_THROTLEINPUT;

	if (m_isThrottleCut) {
		thrustScale = 0.0f;
		m_thrustForce = m_worldThrustDir * thrustScale;
		return;
	}

	m_throttleRatio = m_throttleSmoothValue / MAX_THROTLEINPUT;


	//入力値を非線形にして推力に反映。
	thrustScale = std::pow(m_throttleRatio, 2) * m_baseThrust;

	if (m_isBoostOn)
	{
		thrustScale *= m_boostMultiplier;
	}

	// 推力ベクトルを計算（機体の前方方向に推力をかける）
	m_thrustForce = m_worldThrustDir * thrustScale;
}
