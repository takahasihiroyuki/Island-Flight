#pragma once
#include"BonusItemType.h"
#include "GameTuningSettings.h"
#include"BonusItemEffectContext.h"

struct BonusItemSpawnPoint
{
	std::string name;
	Vector3 position = Vector3::Zero;
};

class CollectibleObject;
class Aircraft;
class ScoreManager;
class Timer;
class BonusItemObject;
class BonusItemWaypointSet;

/// <summary>
/// ボーナスアイテムを管理するか浦須
/// シーンから登録されたスポーン地点をもとにボーナスアイテムを生成または補充する
/// 最初に必要数スポーンし、取られたら非アクティブにする
/// リスポーン時はアクティブにして再利用する
/// 取得されたアイテムには BonusItemEffectContext を渡して効果を発動させる
/// また、生成済みアイテムのモデル情報を InstancingManager に登録する
/// </summary>
class BonusItemManager :public IGameObject
{
public:
	BonusItemManager();
	~BonusItemManager();

	void Init(const GameTuning::BonusItemManagerConfig& config)
	{
		m_config = config;
	}

	bool Start() override;

	void Update() override;

	bool SpawnItem(
		const std::string& objectName,
		const Vector3& position
	);

	/// <summary>
	/// アクティブじゃないアイテムを探して返す
	/// </summary>
	/// <param name="itemName"></param>
	/// <returns></returns>
	BonusItemObject* FindInactiveItem(const std::string& itemName) const;


	/// <summary>
	/// アイテムをアクティブにして指定された場所に置く
	/// </summary>
	/// <param name="itemName"></param>
	/// <param name="position"></param>
	bool ActivateItemAt(
		const std::string& itemName,
		const Vector3& position);

	void SpawnInitialItems();

	void SetGameplayDependencies(
		Aircraft* aircraft,
		ScoreManager* scoreManager
	)
	{
		m_aircraft = aircraft;
		m_effectContext.scoreManager = scoreManager;
	}

	void SetAddTimePopupUI(AddTimePopupUI* addTimePopupUI)
	{
		m_effectContext.addTimePopupUI = addTimePopupUI;
	}

	void SetInstancingManager(InstancingManager* instancingManager)
	{
		m_instancingManager = instancingManager;
	}

	/// <summary>
	/// ボーナスアイテムのモデルをインスタンシングマネージャーに登録する
	/// </summary>
	void RegisterBonusItemInstancingModels();

	void SetGameTimer(Timer* gameTimer)
	{
		m_effectContext.gameTimer = gameTimer;
	}

	void SetWaypointSet(const BonusItemWaypointSet* waypointSet)
	{
		m_waypointSet = waypointSet;
	}

	void SetBonusItemSpawnPoint(const BonusItemSpawnPoint& spawnPoint)
	{
		m_bonusItemSpawnPoints.push_back(spawnPoint);
	}

	static bool IsSupportedItemName(
		const std::string& objectName
	);

private:

	/// <summary>
	/// ボーナスアイテムが取得範囲内にあるか判定する。
	/// </summary>
	bool IsInCollectRange(const BonusItemObject* item) const;

	/// <summary>
	/// 指定された種類のボーナスアイテムを生成する
	/// </summary>
	/// <param name="objectName"></param>
	/// <param name="position"></param>
	/// <returns></returns>
	BonusItemObject* CreateItem(
		const std::string& objectName,
		const Vector3& position);

	/// <summary>
	/// アイテムごとの決められた存在数を維持する
	/// </summary>
	void MaintainItemCounts();

	/// <summary>
	/// していされたアイテムのアクティブになっている数を出す
	/// </summary>
	/// <param name="itemName"></param>
	/// <returns></returns>
	int CountActiveItems(const std::string& itemName) const;

	/// <summary>
	/// 使用可能なスポーン地点を探す。
	/// </summary>
	const BonusItemSpawnPoint* FindSpawnPoint(const std::string& itemName) const;

	/// <summary>
	/// 指定されたアイテムを目標数まで補充する。
	/// </summary>
	/// <param name="itemName"></param>
	/// <param name="targetCount"></param>
	void ReplenishItems(const std::string& itemName, int targetCount);


private:
	Aircraft* m_aircraft = nullptr;
	InstancingManager* m_instancingManager = nullptr;


	std::vector<BonusItemObject*> m_items;
	std::unordered_set<std::string> m_usedBonusItemNameSet;
	std::unordered_map<std::string, std::string> m_bonusItemModelPaths;
	GameTuning::BonusItemManagerConfig m_config;
	BonusItemEffectContext m_effectContext;
	const BonusItemWaypointSet* m_waypointSet = nullptr;
	std::vector<BonusItemSpawnPoint> m_bonusItemSpawnPoints;
};

