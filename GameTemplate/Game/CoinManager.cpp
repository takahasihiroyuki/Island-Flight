#include "stdafx.h"
#include "CoinManager.h"
#include "Coin.h"
#include "Aircraft.h"
#include"ScoreManager.h"

namespace {
	const char* COIN_FILEPATH = "Assets/modelData/coin.tkm";
	const char* COIN_MODELNAME = "Coin";
}

CoinManager::CoinManager()
{

}

CoinManager::~CoinManager()
{
	for (Coin* coin : m_coins) {
		DeleteGO(coin);
	}
	m_coins.clear();

	for (Coin* coin : m_pendingCoins) {
		DeleteGO(coin);
	}
	m_pendingCoins.clear();
}

void CoinManager::Update(const Aircraft& player)
{
	CheckCoinPickup(player);
	ProcessCollectedCoins(player);
}

bool CoinManager::Start()
{
	return true;
}

void CoinManager::Spawn(
	Vector3 position,
	Quaternion rotation,
	Vector3 scale)
{
	Coin* coin = NewGO<Coin>(0, "coin");
	coin->Init(COIN_FILEPATH, position, rotation, scale, "Coin");
	m_coins.push_back(coin);
}

void CoinManager::RequestSpawn(
	Vector3 position,
	Quaternion rotation,
	Vector3 scale)
{
	Coin* coin = NewGO<Coin>(0, "coin");
	coin->Init(COIN_FILEPATH, position, rotation, scale, "Coin");
	m_pendingCoins.push_back(coin);
}

void CoinManager::RegisterCoinInstancingModel()
{
	//インスタンス描画に登録するためのデータを作成。

	//モデルネームの集合を作成。
	std::unordered_set<std::string> modelNameSet;
	modelNameSet.insert(COIN_MODELNAME);

	//モデルパスの集合を作成。
	std::unordered_map<std::string, std::string> paths;
	paths[COIN_MODELNAME] = COIN_FILEPATH;

	std::unordered_map<std::string, bool> instancingFlags;
	instancingFlags[COIN_MODELNAME] = PlacementObject::GetIsInstancing(COIN_MODELNAME);

	std::unordered_map<std::string, size_t> maxInstanceTable;
	maxInstanceTable[COIN_MODELNAME] = PlacementObject::GetMaxInstanceCount(COIN_MODELNAME);


	if (m_instancingManager)
	{
		//インスタンシング描画に登録する。
		m_instancingManager->RegisterInstancingModels(
			modelNameSet,
			paths,
			instancingFlags,
			maxInstanceTable
		);
	}

}

std::vector<size_t> CoinManager::CheckCoinPickup(const Aircraft& player)
{
	//ヒットしたコインのインデックスを格納する配列
	std::vector<size_t> hitIndices;
	hitIndices.reserve(m_coins.size());
	for (size_t i = 0; i < m_coins.size(); ++i) {
		if (!m_coins[i]->IsActive()) continue;

		Vector3 playerToCoinVector = player.GetPosition() - m_coins[i]->GetPosition();
		float coinDist = playerToCoinVector.Length();
		float radius = m_pickupRadius;
		if (coinDist <= radius) {
			hitIndices.push_back(i);

		}
	}
	return hitIndices;
}

void CoinManager::ProcessCollectedCoins(const Aircraft& player)
{
	// 取得されたコインのインデックスを取得
	std::vector<size_t> collectedCoinIndices = CheckCoinPickup(player);

	// 見つかったコインを処理
	for (size_t index : collectedCoinIndices) {
		Coin& coin = *m_coins[index];
		//TODO:スコアを加算
		m_scoreManager->AddScore(m_score);
		//TODO:エフェクト
		//TODO:効果音

		m_coins[index]->Deactivate();

	}
}
