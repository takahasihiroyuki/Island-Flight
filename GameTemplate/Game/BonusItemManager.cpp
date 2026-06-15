#include "stdafx.h"
#include "BonusItemManager.h"
#include "Aircraft.h"
#include "ScoreManager.h"
#include "Timer.h"
#include "PlacementObject.h"
#include "AddTimeItem.h"
#include "ScoreBoostItem.h"


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

	m_deleteRequestItems.clear();
}

bool BonusItemManager::Start()
{

	return false;
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
		if (item->Collect())
		{
			m_deleteRequestItems.push_back(item);
		}

	}

	DeleteCollectedItems();
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

void BonusItemManager::DeleteCollectedItems()
{
	for (BonusItemObject* deleteItem : m_deleteRequestItems)
	{
		auto it = std::find(m_items.begin(), m_items.end(), deleteItem);

		if (it != m_items.end())
		{
			DeleteGO(*it);
			m_items.erase(it);
		}
	}

	m_deleteRequestItems.clear();
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
			m_config.addTime
		);

		return item;
	}

	if (objectName == ScoreBoostItem::GetObjectName())
	{
		ScoreBoostItem* item =
			NewGO<ScoreBoostItem>(0);

		item->InitItem(
			position,
			m_config.scoreBoost
		);

		return item;
	}

	return nullptr;
}
