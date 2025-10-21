#pragma once

class Aircraft;
class Coin;
class CoinDesc;
class ScoreManager;
class CoinManager:public IGameObject
{
public:
	CoinManager();
	~CoinManager();
	void Update(const Aircraft& player);

public:
	void SetScoreManager(ScoreManager* scoreManager) {
		assert(scoreManager != nullptr);
		m_coreManager=scoreManager;
	}

	/// <summary>
	/// コインを作る
	/// </summary>
	void Spawn(const CoinDesc& coinDesc);

	/// <summary>
	/// フレームの最後でコインを作る。
	/// 途中でベクターへ要素を追加すると再配置が発生し、
	/// 既存ポインタの無効化やエラーを引き起こす可能性があるため。
	/// </summary>
	void RequestSpawn(CoinDesc& coinDesc);

private:
	/// <summary>
	/// プレイヤーが取得したコインを判定する。
	/// </summary>
	/// <param name="player">取得判定のためのプレイヤー</param>
	/// <returns>取得されたコインのインデックス番号。</returns>
	std::vector<size_t> CheckCoinPickup(const Aircraft& player);

	/// <summary>
	/// コインが取得されたときの処理。
	/// </summary>
	/// <param name="player"></param>
	void ProcessCollectedCoins(const Aircraft& player);

private:
	float m_pickupRadius = 10;		//コインの取得範囲
	std::vector<Coin*> m_coins;
	std::vector<Coin*> m_pendingCoins;	//
	ScoreManager* m_coreManager;
};
