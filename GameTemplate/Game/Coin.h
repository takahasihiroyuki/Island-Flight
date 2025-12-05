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
		m_coinSE->Play(true);
	}

	void Deactivate() override
	{
		PlacementObject::Deactivate();
		m_coinSE->Stop();
		m_coinGetSE->Stop();
	}

	void PlayCollectEffects()
	{
		m_coinGetSE->Play(false);
	}

private:
	SoundSource* m_coinSE = nullptr;
	SoundSource* m_coinGetSE = nullptr;
	Timer* m_timer = nullptr;
	float m_rotateSpeed = 300.0f;
};

