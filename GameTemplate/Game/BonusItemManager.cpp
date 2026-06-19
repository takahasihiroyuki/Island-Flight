#include "stdafx.h"
#include "BonusItemManager.h"
#include "Aircraft.h"
#include "ScoreManager.h"
#include "Timer.h"
#include "PlacementObject.h"
#include "AddTimeItem.h"
#include "ScoreBoostItem.h"

namespace {
	const float MOVE_DURATION = 10.0f;
}

BonusItemManager::BonusItemManager()
{

}

BonusItemManager::~BonusItemManager()
{
	for (BonusItemObject* item : m_items)
	{
		DeleteGO(item);
	}
	m_items.clear();
}

bool BonusItemManager::Start()
{


	return true;
}

void BonusItemManager::Update()
{
	if (m_aircraft == nullptr) return;


	for (BonusItemObject* item : m_items)
	{
		if (item == nullptr) continue;

		//アイテムが取得範囲内にあるか判定する。
		if (!IsInCollectRange(item))continue;

		//アイテムを取得する。
		item->Collect();

	}

	//アクティブになっている個数を調べて不足していればアクティブにする
	MaintainItemCounts();
}

bool BonusItemManager::SpawnItem(const std::string& objectName, const Vector3& position)
{

	BonusItemObject* item = CreateItem(objectName, position);

	if (item == nullptr)
	{
		return false;
	}

	item->SetEffectContext(&m_effectContext);

	m_items.push_back(item);

	const std::string modelName = item->GetItemModelName();

	const std::string modelPath = item->GetItemModelPath();

	// インスタンシング登録用に記録する
	m_usedBonusItemNameSet.insert(modelName);
	m_bonusItemModelPaths[modelName] = modelPath;

	return true;
}

BonusItemObject* BonusItemManager::FindInactiveItem(const std::string& itemName) const
{
	for (BonusItemObject* item : m_items)
	{
		if (item == nullptr)
		{
			continue;
		}

		if (!item->IsActive() && itemName == item->GetItemModelName())
		{
			return item;
		}
	}

	return nullptr;
}


bool BonusItemManager::ActivateItemAt(const std::string& itemName, const Vector3& position)
{
	BonusItemObject* item = FindInactiveItem(itemName);

	if (item == nullptr)
	{
		return false;
	}

	item->SetPosition(position);
	item->InitMovement(
		m_waypointSet,
		position,
		MOVE_DURATION
	);
	item->Activate();

	return true;
}

void BonusItemManager::SpawnInitialItems()
{
	MaintainItemCounts();
}

void BonusItemManager::RegisterBonusItemInstancingModels()
{
	if (m_instancingManager == nullptr)
	{
		return;
	}

	if (m_usedBonusItemNameSet.empty())
	{
		return;
	}

	std::unordered_map<std::string, bool> instancingFlags;
	std::unordered_map<std::string, size_t> maxInstanceTable;

	for (const auto& modelName : m_usedBonusItemNameSet)
	{
		instancingFlags[modelName] = PlacementObject::GetIsInstancing(modelName.c_str());
		maxInstanceTable[modelName] = PlacementObject::GetMaxInstanceCount(modelName.c_str());
	}

	m_instancingManager->RegisterInstancingModels(
		m_usedBonusItemNameSet,
		m_bonusItemModelPaths,
		instancingFlags,
		maxInstanceTable
	);
}

bool BonusItemManager::IsSupportedItemName(const std::string& objectName)
{
	return
		objectName == AddTimeItem::GetObjectName() ||
		objectName == ScoreBoostItem::GetObjectName();
}

bool BonusItemManager::IsInCollectRange(const BonusItemObject* item) const
{
	if (item == nullptr || m_aircraft == nullptr)
	{
		return false;
	}

	const Vector3 toItem = item->GetPosition() - m_aircraft->GetPosition();
	const float distanceSq = toItem.LengthSq();

	return distanceSq <= m_config.collectRadius * m_config.collectRadius;
}

BonusItemObject* BonusItemManager::CreateItem(const std::string& objectName, const Vector3& position)
{
	//objectNameに対応するアイテムを生成する。
	if (objectName == AddTimeItem::GetObjectName())
	{
		AddTimeItem* item =
			NewGO<AddTimeItem>(0);

		item->InitItem(
			position,
			m_config.addTime,
			m_waypointSet,
			position,
			MOVE_DURATION
		);

		return item;
	}

	if (objectName == ScoreBoostItem::GetObjectName())
	{
		ScoreBoostItem* item =
			NewGO<ScoreBoostItem>(0);

		item->InitItem(
			position,
			m_config.scoreBoost,
			m_waypointSet,
			position,
			MOVE_DURATION
		);

		return item;
	}

	return nullptr;
}

void BonusItemManager::MaintainItemCounts()
{
	ReplenishItems(
		"AddTimeItem",
		m_config.addTime.itemActiveCount
	);

	ReplenishItems(
		"ScoreBoostItem",
		m_config.scoreBoost.itemActiveCount
	);
}

int BonusItemManager::CountActiveItems(const std::string& itemName) const
{
	int activeItemCount = 0;

	for (const BonusItemObject* item : m_items)
	{
		if (item == nullptr)
		{
			continue;
		}

		if (item->IsActive() && itemName == item->GetItemModelName())
		{
			activeItemCount++;
		}
	}

	return activeItemCount;
}

const BonusItemSpawnPoint* BonusItemManager::FindSpawnPoint(const std::string& itemName) const
{
	//スポーン可能なポイントの候補
	std::vector<const BonusItemSpawnPoint*> candidates;

	for (const BonusItemSpawnPoint& spawnPoint : m_bonusItemSpawnPoints)
	{
		//スポーン地点とアイテムネームがあっていれば候補に入れる
		if (spawnPoint.name == itemName)
		{
			candidates.push_back(&spawnPoint);
		}

	}

	if (candidates.empty())
	{
		return nullptr;
	}

	const size_t randomIndex = static_cast<size_t>(std::rand()) % candidates.size();

	//候補の中からランダムで選ぶ
	return candidates[randomIndex];
}

void BonusItemManager::ReplenishItems(const std::string& itemName, int targetCount)
{
	//今の存在しているアイテム数
	int currentCount = CountActiveItems(itemName);

	while (currentCount < targetCount)
	{
		//スポーンポイントを探す
		const BonusItemSpawnPoint* spawnPoint = FindSpawnPoint(itemName);

		if (spawnPoint == nullptr)
		{
			// 対応するスポーン地点が登録されていない。
			break;
		}

		//生成済みで非アクティブのものがあれば使いまわす
		if (ActivateItemAt(itemName, spawnPoint->position))
		{
			currentCount++;
			continue;
		}

		//新しくスポーンする
		const bool isSpawned = SpawnItem(itemName, spawnPoint->position);

		if (!isSpawned)
		{
			// アイテムの生成に失敗した。
			break;
		}

		currentCount++;
	}
}