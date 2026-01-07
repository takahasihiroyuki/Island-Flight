#pragma once
#include"PlacementObject.h"

// ¶¬—p‚ÌÝŒv}
struct CoinDesc {
	Vector3    pos;
	Quaternion rot;
	Vector3    scale;
};

struct CoinDesc;
class Timer;
class Coin : public ItemMeshObject
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

	void Deactivate() override
	{
		PlacementObject::Deactivate();
		if (m_coinSE) {
			m_coinSE->Stop();
		}
		if (m_coinGetSE) {
			m_coinGetSE->Stop();
		}
	}

	void PlayCollectEffects();

private:
	SoundSource* m_coinSE = nullptr;
	SoundSource* m_coinGetSE = nullptr;
	Timer* m_timer = nullptr;
	float m_rotateSpeed = 300.0f;
	EffectEmitter* m_collectEffect = nullptr;
};

