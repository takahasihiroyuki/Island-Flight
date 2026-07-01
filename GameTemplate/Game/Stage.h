#pragma once
#include "AssistWarpPoint.h"
#include "BonusItemEffectContext.h"
#include "BonusItemWaypointSet.h"

class StageMeshObject;
class CoinManager;
class PlacementObject;
class BonusItemManager;
class Stage :public IGameObject
{
public:
	Stage() {
		float debug = 0;
	};
	~Stage();

	void Init();
	bool Start() override;

	void Update() {
	}

	Vector3 GetPlayerStartPos() const
	{
		return m_playerStartPos;
	}

	std::vector<AssistWarpPoint> GetAssistWarpPoints() {
		return m_assistWarpPoint;
	}

	/// <summary>
	/// フォグのパラメータを設定
	/// </summary>
	/// <param name="collar"></param>
	/// <param name="fogDistanceScale"></param>
	void SetFogParams(Vector3 collar, float fogDistanceScale) {
		g_renderingEngine->SetFogParams(collar, fogDistanceScale);
	}


	void SetCoinManager(CoinManager* coinManager)
	{
		m_coinManager = coinManager;
	}

	void SetBonusItemManager(BonusItemManager* bonusItemManager)
	{
		m_bonusItemManager = bonusItemManager;
	}

	const BonusItemWaypointSet* GetBonusItemWaypointSet() const
	{
		return &m_bonusItemWaypointSet;
	}




private:
	CoinManager* m_coinManager = nullptr;
	BonusItemManager* m_bonusItemManager = nullptr;
	InstancingManager* m_instancingManager = nullptr;
	Vector3 m_posOfset = Vector3::Zero;
	Vector3 m_ScaleOfset;
	Vector3 m_playerStartPos;
	std::vector<PlacementObject*> m_stageObjects;
	std::vector<AssistWarpPoint> m_assistWarpPoint;
	BonusItemWaypointSet m_bonusItemWaypointSet;
};

