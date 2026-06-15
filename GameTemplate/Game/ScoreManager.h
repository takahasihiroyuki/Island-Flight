#pragma once
#include"ComboSystem.h"
#include "GameTuningSettings.h"
#include "ScoreBoostEffect.h"

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

	void StartScoreBoost(float multiplier, float duration) {
		m_scoreBoostEffect.Start(multiplier, duration);
	}

	/// <summary>
	/// コインを取ったときのスコア加算処理。
	/// </summary>
	/// <param name="baseScore"></param>
	float OnCoinCollected(float baseScore)
	{
		m_comboSystem.AddCombo();

		// コンボ倍率を取得
		const float comboMultiplier = m_comboSystem.GetComboMultiplier();

		// スコアブーストの倍率を取得
		const float scoreBoostMultiplier = m_scoreBoostEffect.GetMultiplier();

		m_score += baseScore * comboMultiplier * scoreBoostMultiplier;
		return baseScore * comboMultiplier * scoreBoostMultiplier;
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
	ScoreBoostEffect m_scoreBoostEffect;
};

