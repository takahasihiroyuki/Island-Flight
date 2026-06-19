#pragma once
#include"PlacementObject.h"
class Timer;
class ScoreManager;
class Coin : public CollectibleObject
{
public:
	Coin();
	~Coin();
	void OnStart() override;
	void OnInit()  override {}
	void OnUpdate() override;
	void Activate() override
	{
		PlacementObject::Activate();
		if (m_coinSE) {
			m_coinSE->Play(true);
		}
	}

	void OnDeactivate() override
	{
		if (m_coinSE) {
			m_coinSE->Stop();
		}
		if (m_coinGetSE) {
			m_coinGetSE->Stop();
		}
	}

	void OnCollected(int comboCount);

	void PlayCollectEffects(int comboCount);

	void SetBaseScore(float score)
	{
		m_baseScore = score;
	}

private:
	float GetCollectSEFrequencyRatio(int comboCount) const;

private:
	SoundSource* m_coinSE = nullptr;
	SoundSource* m_coinGetSE = nullptr;
	Timer* m_timer = nullptr;
	float m_rotateSpeed = 300.0f;
	float m_baseScore = 100.0f;
	EffectEmitter* m_collectEffect = nullptr;

};

