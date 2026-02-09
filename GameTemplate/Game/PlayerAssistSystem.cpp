#include "stdafx.h"
#include "PlayerAssistSystem.h"
#include "Aircraft.h"
#include "CameraManager.h"


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
