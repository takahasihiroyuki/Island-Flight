#pragma once
#include"ComboSystem.h"
class ScoreManager :public IGameObject
{
public:
	ScoreManager();
	~ScoreManager();
	void Init(const GameTuning::ScoreManagerConfig& settings) {
		m_comboSystem.Init(settings.combo);
	}
	void Update() override
	{
		m_comboSystem.Update();
	}

	/// <summary>
	/// コインを取ったときのスコア加算処理。
	/// </summary>
	/// <param name="baseScore"></param>
	float OnCoinCollected(float baseScore)
	{
		m_comboSystem.AddCombo();
		// コンボ倍率を取得
		float comboMultiplier = m_comboSystem.GetComboMultiplier();
		m_score += baseScore * comboMultiplier;
		return baseScore * comboMultiplier;
	}

	float GetScore() {
		return m_score;
	}

	/// <summary>
	/// 今のコンボ数を返す
	/// </summary>
	/// <returns></returns>
	int GetComboCount() const
	{
		return m_comboSystem.GetComboCount();
	}

	/// <summary>
	/// コンボの残り時間の割合を返す
	/// </summary>
	/// <returns></returns>
	float GetComboRemainingRate() const
	{
		return m_comboSystem.GetComboRemainingRate();
	}

	/// <summary>
	/// コンボが続いているかを返す
	/// </summary>
	/// <returns></returns>
	bool IsComboActive() const
	{
		return m_comboSystem.IsComboActive();
	}

private:
	float m_score = 0;
	ComboSystem m_comboSystem;
};

