#pragma once
class Timer :public IGameObject
{
public:
	Timer() {}
	~Timer() {}
	void Init(float limitTime)
	{
		m_limitTime = limitTime;
	}

	void Update()
	{
		if (m_stop)return;
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

	void SetIsStop(bool isStop) 
	{
		m_stop = isStop;
	}

private:
	float m_limitTime = 0;
	float m_elapsedTime = 0;
	bool m_stop = false;
};

