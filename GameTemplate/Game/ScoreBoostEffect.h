#pragma once
#include "Timer.h"

struct ScoreBoostStatus
{
	bool isActive = false;
	float multiplier = 1.0f;
	float remainingSeconds = 0.0f;
	float remainingRate = 0.0f;
};

class ScoreBoostEffect
{
public:
	void Start(float multiplier, float duration)
	{
		m_multiplier = multiplier;
		m_duration = duration;

		m_timer.SetLimitTime(m_duration);
		m_timer.Reset();
		m_timer.SetRunning(true);
		m_activationVersion++;
	}

	void Update()
	{
		if (!m_timer.IsRunning())
		{
			return;
		}

		m_timer.Update();

		if (m_timer.IsTimeUp())
		{
			Stop();
		}
	}

	void Stop()
	{
		m_multiplier = 1.0f;
		m_timer.SetRunning(false);
		m_timer.Reset();
	}

	ScoreBoostStatus GetStatus() const
	{
		if (!m_timer.IsRunning() || m_duration <= 0.0f)
		{
			return {};
		}

		const float remainingSeconds =
			(std::max)(0.0f, m_duration - m_timer.GetElapsedTime());

		return
		{
			true,
			m_multiplier,
			remainingSeconds,
			remainingSeconds / m_duration
		};
	}

	float GetMultiplier() const
	{
		return m_multiplier;
	}

	std::uint32_t GetActivationVersion() const
	{
		return m_activationVersion;
	}

	bool IsScoreBoostActive()  const
	{
		return m_timer.IsRunning() && m_duration > 0.0f;
	}

	float GetScoreBoostMultiplier() const
	{
		return IsScoreBoostActive()
			? m_multiplier
			: 1.0f;
	}

	float GetScoreBoostRemainingTime() const
	{
		return m_timer.GetRemainingTime();
	}

	float GetScoreBoostRemainingRate() const
	{
		if (m_duration <= 0.0f)
		{
			return 0.0f;
		}

		return GetScoreBoostRemainingTime() / m_duration;
	}

	std::uint32_t GetScoreBoostActivationVersion() const
	{
		return m_activationVersion;
	}


private:
	float m_multiplier = 1.0f;
	float m_duration = 0.0f;
	Timer m_timer;
	std::uint32_t m_activationVersion = 0;
};

