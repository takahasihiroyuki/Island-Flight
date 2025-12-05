#include "stdafx.h"
#include "Coin.h"
#include"CoinManager.h"
#include"Timer.h"

Coin::Coin()
{
	m_coinSE = NewGO<SoundSource>(0);
	m_coinSE->Init(static_cast<int>(SoundID::enCoinSE), true);
	m_coinGetSE = NewGO<SoundSource>(0);
	m_coinGetSE->Init(static_cast<int>(SoundID::enCoinGetSE), false);
}

Coin::~Coin()
{
	DeleteGO(m_coinSE);
	DeleteGO(m_coinGetSE);
}

void Coin::OnStart()
{

}

void Coin::OnUpdate()
{


	Quaternion rot;
	float deltaTime = g_gameTime->GetFrameDeltaTime();
	m_rotation.AddRotationDegY(deltaTime * m_rotateSpeed);


	// ƒRƒCƒ“‚ÌˆÊ’u‚ÉSE‚ð’Ç]‚³‚¹‚é
	m_coinSE->SetPosition(GetPosition());
}
