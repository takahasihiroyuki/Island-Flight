#pragma once
#include "GameTuningSettings.h"
#include "Timer.h"
/// <summary>
/// コンボの状態を管理するクラス。
/// コンボ数、コンボ継続時間、スコア倍率の計算を担当する。
/// </summary>
class ComboSystem : public IGameObject
{
public:
	ComboSystem() = default;
	~ComboSystem() = default;

	void Init(const GameTuning::ComboConfig& settings) {
		m_comboDuration = settings.duration;
		m_scoreMaxMultiplier = settings.maxMultiplier;
		// コンボタイマーの初期化
		m_comboTimer.SetLimitTime(m_comboDuration);
		m_comboTimer.SetRunning(false);
		m_comboTimer.Reset();
	}

	void Update() override;
	void AddCombo();
	void ResetCombo();


	int GetComboCount() const
	{
		return m_comboCount;
	}

	float GetComboMultiplier() const
	{
		return CalculateMultiplier();
	}

	float GetScoreMaxMultiplier() const
	{
		return m_scoreMaxMultiplier;
	}

	/// <summary>
	/// コンボ中か？
	/// </summary>
	/// <returns></returns>
	bool IsComboActive() const
	{
		return m_comboCount > 0 && m_comboTimer.IsRunning();
	}

	/// <summary>
	/// コンボが切れるまでの残り時間を返す。
	/// </summary>
	/// <returns></returns>
	float GetComboRemainingTime() const
	{
		if (!IsComboActive())
		{
			return 0.0f;
		}

		float remainingTime = m_comboDuration - m_comboTimer.GetElapsedTime();

		if (remainingTime < 0.0f)
		{
			remainingTime = 0.0f;
		}

		return remainingTime;
	}

	/// <summary>
	/// コンボが切れるまでの残り時間の割合を返す。0.0f～1.0fの範囲で返す。
	/// </summary>
	/// <returns></returns>
	float GetComboRemainingRate() const
	{
		if (!IsComboActive())
		{
			return 0.0f;
		}

		if (m_comboDuration <= 0.0f)
		{
			return 0.0f;
		}

		return GetComboRemainingTime() / m_comboDuration;
	}

private:
	float CalculateMultiplier() const
	{
		// コンボ数に応じてスコア倍率を計算
		float currentMultiplier = 1.0f + ((m_comboCount - 1) * 0.5f); //コンボ数が増えるごとに倍率が増加
		if (currentMultiplier > m_scoreMaxMultiplier) {
			currentMultiplier = m_scoreMaxMultiplier; //最大倍率を超えないようにする
		}
		return currentMultiplier;
	}

private:
	int m_comboCount = 0;					// 現在のコンボ数
	Timer m_comboTimer;						// コンボ継続時間のタイマー
	float m_comboDuration = 3.0f;			// コンボが継続する時間（秒）
	float m_scoreMaxMultiplier = 1.0f;		// 最大スコア倍率
};

