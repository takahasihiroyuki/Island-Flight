#include "stdafx.h"
#include <algorithm>
#include <random>
#include "CoinManager.h"
#include "Coin.h"
#include "Aircraft.h"
#include "ScoreManager.h"
#include "ScorePopupScreen.h"

namespace {
	static const char* COIN_FILEPATH = "Assets/modelData/coin.tkm";
	static const char* COIN_MODELNAME = "Coin";
	static constexpr size_t INITIAL_COIN_COUNT = 50;			//コインの初期枚数
	const Vector3 SCORE_POPUP_POS = Vector3(300.0f, 100.0f, 0.0f);

	float RandomFloat(float min, float max)
	{
		//シード地を生成
		static std::random_device rd;

		//乱数生成器を生成
		static std::mt19937 gen(rd());

		//分布を作成
		//確率に偏りがない一様分布を使用
		std::uniform_real_distribution<float> dist(min, max);

		//乱数を生成して返す
		return dist(gen);
	}
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
	//コインが取られたかチェックする
	CheckCoinPickup(player);
	//コイン
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
	//最初は非アクティブにしておく
	coin->Deactivate();
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

std::vector<Vector3> CoinManager::GetPlayerToCoinVecs(const Vector3& playerPos, float maxDistance, size_t maxArrowCount)
{
	std::vector<Vector3> out;
	out.reserve(min(m_coins.size(), maxArrowCount));// 再確保を防ぐため最大数分確保
	float maxDistanceSq = maxDistance * maxDistance; // 距離の二乗で比較するため変換

	for (auto& coin : m_coins)
	{
		if (!coin->IsActive()) continue;

		Vector3 playerToCoin = coin->GetPosition() - playerPos;
		if (playerToCoin.LengthSq() > maxDistanceSq) continue;
		out.push_back(playerToCoin);
	}

	// 距離の近い順にソート
	std::sort(out.begin(), out.end(),
		[](const Vector3& a, const Vector3& b) {
			return a.LengthSq() < b.LengthSq();// Lengthは重いので距離の二乗で比較
		});

	// 最大数を超えていたら遠いものから順に削る
	if (out.size() > maxArrowCount)
	{
		out.resize(maxArrowCount);
	}

	return out;
}

void CoinManager::ActivateInitialCoins()
{
	std::vector<size_t> indices(m_coins.size());
	for (size_t i = 0; i < indices.size(); ++i)
	{
		indices[i] = i;
	}

	// シャッフルして先頭 count だけ使う
	std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device{}()));
	for (size_t i = 0; i < INITIAL_COIN_COUNT && i < indices.size(); ++i)
	{
		m_coins[indices[i]]->Activate();
	}

}

bool CoinManager::CheckCoinPickup(const Aircraft& player)
{
	bool isGet = false;

	m_hitIndices.clear();
	m_hitIndices.reserve(m_coins.size());

	for (size_t i = 0; i < m_coins.size(); ++i) {

		//非アクティブなコインはスキップ
		if (!m_coins[i]->IsActive()) continue;

		Vector3 coinpos = m_coins[i]->GetPosition();
		Vector3 playerToCoinVector = player.GetPosition() - coinpos;
		float coinDist = playerToCoinVector.Length();
		float radius = m_pickupRadius;

		if (coinDist <= radius) {//コインが取られた
			m_hitIndices.push_back(i);
			isGet = true;
		}
	}

	return isGet;
}

void CoinManager::ProcessCollectedCoins(const Aircraft& player)
{
	// 見つかったコインを処理
	for (size_t index : m_hitIndices) {
		Coin& coin = *m_coins[index];

		m_coinCount++;

		//スコアを加算
		float addScore = m_scoreManager->OnCoinCollected(m_coinBaseScore);
		//コイン取得時の処理を書く
		m_coins[index]->OnCollected();

		//アクティブじゃないコインを1つ選んでアクティブにする
		SelectActive(player.GetPosition(), player.GetLinearVelocity());

		//スコアポップアップを表示
		if (m_scorePopupScreen != nullptr)
		{
			m_scorePopupScreen->ShowAddScore(
				static_cast<int>(addScore),
				SCORE_POPUP_POS
			);
		}

		m_coins[index]->Deactivate();

	}
}

void CoinManager::SelectActive(const Vector3& playerPos, const Vector3& playerForward)
{
	std::vector<float> weights;		// 確率の重み付け用
	std::vector<size_t> indices;	// 候補コインのインデックス

	// プレイヤーの正面方向を正規化
	Vector3 playerForwardDir = playerForward;
	playerForwardDir.Normalize();

	// アクティブでないコインを列挙し、重みを計算
	for (size_t i = 0; i < m_coins.size(); ++i)
	{
		// アクティブなコインはスキップ
		if (m_coins[i]->IsActive()) continue;

		// プレイヤーからコインへのベクトル
		Vector3 playerToCoin = m_coins[i]->GetPosition() - playerPos;

		//プレイヤーとコインの距離
		float dist = playerToCoin.Length();

		//正規化
		playerToCoin.Normalize();

		//どれだけプレイヤーがそのコインの方向を向いているか
		//大きいほどそのコインは選ばれやすい
		float forwardFactor = Dot(playerForwardDir, playerToCoin);
		forwardFactor = (forwardFactor + 1.0f) / 2.0f; // -1~1を0~1に変換
		forwardFactor += 1;//真後ろ（0）も候補に入れるため１を足す

		//どれだけプレイヤーから離れているか
		//距離が近いほどそのコインは選ばれやすい
		float distanceFactor = 1.0 / (dist + 1);//0割対策のために分母に1を足している

		//重みを計算
		float weight = forwardFactor * distanceFactor;

		indices.push_back(i);
		weights.push_back(weight);
	}
	//重みの合計を計算
	float totalWeight = 0.0f;
	for (float weight : weights)
	{
		totalWeight += weight;
	}

	if (weights.empty())
		return;
	if (totalWeight <= 0.0f)
		return;

	//0から合計値までのランダムな値を生成
	float randValue = RandomFloat(0, totalWeight);

	size_t chosen = 0;

	//重みをひとつづつ足していって、ランダム値を超えたらそのコインを選ぶ
	float cumulativeWeight = 0.0f;
	for (size_t i = 0; i < weights.size(); ++i)
	{
		cumulativeWeight += weights[i];
		if (randValue <= cumulativeWeight)
		{
			chosen = i;
			break;
		}
	}
	size_t coinIndex = indices[chosen];
	//選ばれたコインをアクティブにする
	m_coins[coinIndex]->Activate();
}