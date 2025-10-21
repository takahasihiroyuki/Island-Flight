#include "stdafx.h"
#include "CoinManager.h"
#include "Coin.h"
#include "Aircraft.h"
#include"ScoreManager.h"

namespace {
	constexpr float COIN_PICKUPRADIUS = 10.0f;	//コインの取得範囲
}

CoinManager::CoinManager()
{

}

CoinManager::~CoinManager()
{

}

void CoinManager::Update(const Aircraft& player)
{
	CheckCoinPickup(player);
	ProcessCollectedCoins(player);
}

void CoinManager::Spawn(const CoinDesc& coinDesc)
{
	Coin* coin = NewGO<Coin>(0, "coin");
	coin->Initialize(coinDesc);
	m_coins.push_back(coin);
}

void CoinManager::RequestSpawn(CoinDesc& coinDesc)
{
	Coin* coin = NewGO<Coin>(0, "coin");
	coin->Initialize(coinDesc);
	m_pendingCoins.push_back(coin);
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
		float radius = COIN_PICKUPRADIUS;
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
		m_coins[index]->Deactivate();
		//TODO:スコアを加算
		//TODO:エフェクト
		//TODO:効果音
	}
}
