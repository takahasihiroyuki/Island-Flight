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


public:

	/// <summary>
	/// コインを取ったときのスコア加算処理。
	/// </summary>
	/// <param name="baseScore"></param>
	void OnCoinCollected(float baseScore)
	{
		m_comboSystem.AddCombo();
		// コンボ倍率を取得
		float comboMultiplier = m_comboSystem.GetComboMultiplier();
		m_score += baseScore * comboMultiplier;
	}

	float GetScore() {
		return m_score;
	}

private:
	float m_score = 0;
	ComboSystem m_comboSystem;
};

