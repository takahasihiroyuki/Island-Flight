#pragma once

class Aircraft;
class Coin;
class CoinDesc;
class ScoreManager;
class CoinManager :public IGameObject
{
public:
	CoinManager();
	~CoinManager();
	void Update(const Aircraft& player);
	bool Start()override;

public:
	void SetScoreManager(ScoreManager* scoreManager) {
		assert(scoreManager != nullptr);
		m_scoreManager = scoreManager;
	}

	/// <summary>
	/// コインを作る
	/// </summary>
	void Spawn(
		Vector3 position,
		Quaternion rotation,
		Vector3 scale);

	/// <summary>
	/// フレームの最後でコインを作る。
	/// 途中でベクターへ要素を追加すると再配置が発生し、
	/// 既存ポインタの無効化やエラーを引き起こす可能性があるため。
	/// </summary>
	void RequestSpawn(
		Vector3 position,
		Quaternion rotation,
		Vector3 scale);

	void SetInstancingManager(InstancingManager* instancingManager)
	{
		m_instancingManager = instancingManager;
	}

	void RegisterCoinInstancingModel();
private:
	/// <summary>
	/// プレイヤーが取得したコインを判定する。
	/// 取られたコインのインデックスを記録する。
	/// </summary>
	/// <param name="player"></param>
	/// <returns></returns>
	bool CheckCoinPickup(const Aircraft& player);

	/// <summary>
	/// コインが取得されたときの処理。
	/// </summary>
	/// <param name="player"></param>
	void ProcessCollectedCoins(const Aircraft& player);

private:
	float m_pickupRadius = 2000.0f;		//コインの取得範囲
	std::vector<Coin*> m_coins;
	std::vector<Coin*> m_pendingCoins;	//
	ScoreManager* m_scoreManager;
	InstancingManager* m_instancingManager = nullptr;
	std::vector<size_t> m_hitIndices;//取られたコインのインデックス
	float m_score = 100.0f;
};
