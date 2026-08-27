#include "Source/stdafx.h"
#include "Source/Gameplay/Assist/PlayerAssistSystem.h"
#include "Source/Gameplay/Aircraft/Aircraft.h"
#include "Source/Camera/CameraManager.h"
#include "Source/UI/UIManager.h"


PlayerAssistSystem::~PlayerAssistSystem()
{
	m_playerAssistUI.reset();
	UIManager::GetInstance().RequestUnregisterScreen("PlayerAssistUI");
}

void PlayerAssistSystem::Update(Aircraft& aircraft)
{
	if (!ShouldAssist())return;

	Vector3 playerPos = aircraft.GetPosition();
	AssistWarpPoint bestWarpPoint = FindBestWarpPoint(playerPos);
	aircraft.RequestWarp(bestWarpPoint.position, bestWarpPoint.rotation);

	// カメラにワープ通知
	{
		//ターゲットのスナップショットを作成
		TargetSnapshot targetSnapshot;
		targetSnapshot.SetPosition(aircraft.GetPosition());
		targetSnapshot.SetRotation(aircraft.GetOrientation());

		//カメラマネージャーにワープを通知
		CameraManager::GetInstance().NotifyTargetWarped(targetSnapshot);
	}
}

void PlayerAssistSystem::Init(std::vector<AssistWarpPoint> warpPoint)
{
	m_warpPoints = warpPoint;

	// プレイヤーアシストUIの初期化
	{
		m_playerAssistUI = std::make_unique<PlayerAssistUI>();
		m_playerAssistUI->Init();
		UIManager::GetInstance().RegisterScreen("PlayerAssistUI", std::move(m_playerAssistUI));
		UIManager::GetInstance().ShowScreen("PlayerAssistUI");
	}

}

bool PlayerAssistSystem::ShouldAssist()
{
	bool shouldAssist = g_pad[0]->IsPress(enButtonY);
	return shouldAssist;
}

AssistWarpPoint PlayerAssistSystem::FindBestWarpPoint(Vector3 playerPosition)
{
	AssistWarpPoint bestPoint = m_warpPoints[0];
	float bestDistanceSq = (playerPosition - bestPoint.position).LengthSq();
	for (const auto& point : m_warpPoints)
	{
		float distanceSq = (playerPosition - point.position).LengthSq();
		if (distanceSq < bestDistanceSq)
		{
			bestDistanceSq = distanceSq;
			bestPoint = point;
		}
	}
	return bestPoint;
}
