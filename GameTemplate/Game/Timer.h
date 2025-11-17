#pragma once
class Timer :public IGameObject
{
public:
	Timer() {}
	~Timer() {}

	/// <summary>
	/// 制限時間を設定。
	/// 呼ばないならカウントダウン、カウントアップをしない。
	/// </summary>
	/// <param name="limitTime"></param>
	void SetLimitTime(float limitTime)
	{
		m_limitTime = limitTime;
		m_isCountingDown = true;
	}

	void Update()
	{
		if (!m_isRunning)return;
		m_elapsedTime += g_gameTime->GetFrameDeltaTime();
	}

	bool IsTimeUp() const
	{
		return m_elapsedTime >= m_limitTime;
	}

	float GetElapsedTime()const
	{
		return m_elapsedTime;
	}

	void Reset()
	{
		m_elapsedTime = 0;
	}

	void SetRunning(bool isRunning)
	{
		m_isRunning = isRunning;
	}

	/// <summary>
	/// 残り時間を返す
	/// カウントダウンをしないなら0を返す。
	/// </summary>
	float GetRemainingTime()
	{
		if (m_isCountingDown) {
			return m_limitTime - m_elapsedTime;
		}

		return 0.0f;
	}

private:
	float m_limitTime = 0;
	float m_elapsedTime = 0;
	bool m_isRunning = false;
	bool m_isCountingDown = false;
};

