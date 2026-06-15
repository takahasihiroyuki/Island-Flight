#pragma once
#include"BonusItemType.h"
#include "GameTuningSettings.h"
#include"BonusItemEffectContext.h"

class CollectibleObject;
class Aircraft;
class ScoreManager;
class Timer;
class BonusItemObject;
class BonusItemManager :public IGameObject
{
public:
	BonusItemManager();
	~BonusItemManager();

	void Init(
		const GameTuning::BonusItemManagerConfig& config)
	{
		m_config = config;
	}

	bool Start() override;

	void Update() override;

	bool SpawnItem(
		const std::string& objectName,
		const Vector3& position
	);

	void SetGameplayDependencies(
		Aircraft* aircraft,
		ScoreManager* scoreManager
	)
	{
		m_aircraft = aircraft;
		m_scoreManager = scoreManager;
	}

	void SetInstancingManager(InstancingManager* instancingManager)
	{
		m_instancingManager = instancingManager;
	}

	void RegisterBonusItemInstancingModels();

	void SetGameTimer(Timer* gameTimer)
	{
		m_effectContext.gameTimer = gameTimer;
	}

private:

	/// <summary>
	/// ボーナスアイテムが取得範囲内にあるか判定する。
	/// </summary>
	bool IsInCollectRange(const BonusItemObject* item) const;


	/// <summary>
	/// 取得済みアイテムを削除する。
	/// </summary>
	void DeleteCollectedItems();

	/// <summary>
	/// 指定された種類のボーナスアイテムを生成する
	/// </summary>
	/// <param name="objectName"></param>
	/// <param name="position"></param>
	/// <returns></returns>
	BonusItemObject* CreateItem(
		const std::string& objectName,
		const Vector3& position);


private:
	Aircraft* m_aircraft = nullptr;
	ScoreManager* m_scoreManager = nullptr;
	InstancingManager* m_instancingManager = nullptr;


	std::vector<BonusItemObject*> m_items;
	std::vector<BonusItemObject*> m_deleteRequestItems;
	std::unordered_set<std::string> m_usedBonusItemNameSet;
	std::unordered_map<std::string, std::string> m_bonusItemModelPaths;
	GameTuning::BonusItemManagerConfig m_config;
	BonusItemEffectContext m_effectContext;
};

