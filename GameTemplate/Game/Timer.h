#pragma once
class Timer :public IGameObject
{
public:
	Timer() {}
	~Timer() {}

	/// <summary>
	/// 制限時間を設定。
	/// 設定すると IsTimeUp() や GetRemainingTime() が使えるようになる。
	/// 設定しない場合でも、GetElapsedTime() で経過時間は取得できる。	/// </summary>
	/// <param name="limitTime"></param>
	void SetLimitTime(float limitTime)
	{
		m_limitTime = limitTime;
		m_hasLimitTime = true;
	}

	void Update()
	{
		if (!m_isRunning)return;

		m_elapsedTime += g_gameTime->GetFrameDeltaTime();
	}

	bool IsTimeUp() const
	{
		if (!m_isRunning) return false;

		if (!m_hasLimitTime) return false;

		return m_elapsedTime >= m_limitTime;
	}

	bool IsRunning() const
	{
		return m_isRunning;
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
		if (!m_hasLimitTime) return 0.0f;

		float remainingTime = m_limitTime - m_elapsedTime;

		if (remainingTime < 0.0f) return 0.0f;

		return remainingTime;
	}

	void AddLimitTime(float addTime)
	{
		if (!m_hasLimitTime) return;

		m_limitTime += addTime;

		if (m_limitTime < 0.0f)
		{
			m_limitTime = 0.0f;
		}
	}

private:
	float m_limitTime = 0;
	float m_elapsedTime = 0;
	bool m_isRunning = false;
	bool m_hasLimitTime = false;
};

