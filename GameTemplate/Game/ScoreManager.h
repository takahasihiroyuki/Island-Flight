#pragma once
class ScoreManager:public IGameObject
{
public:
	ScoreManager();
	~ScoreManager();


public:
	void AddScore(float score) 
	{
		m_score += score;
	};

	float GetScore() {
		return m_score;
	}

private:
	float m_score = 0;


};

