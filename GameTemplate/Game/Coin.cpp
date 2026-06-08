#include "stdafx.h"
#include "Coin.h"
#include"CoinManager.h"
#include"Timer.h"
#include"EffectType.h"

Coin::Coin()
{

}

Coin::~Coin()
{
	DeleteGO(m_coinSE);
	DeleteGO(m_coinGetSE);
}

void Coin::OnStart()
{
	m_coinSE = NewGO<SoundSource>(0);
	m_coinSE->Init(static_cast<int>(SoundID::enCoinSE), true);
	m_coinSE->Play(true);
	m_coinGetSE = NewGO<SoundSource>(0);
	m_coinGetSE->Init(static_cast<int>(SoundID::enCoinGetSE), false);
	m_coinGetSE->Stop();

	//エフェクトの初期化
	EffectEngine::GetInstance()->ResistEffect(enCoinGet, effectPath[enCoinGet]);

}

void Coin::OnUpdate()
{

	Quaternion rot;
	float deltaTime = g_gameTime->GetFrameDeltaTime();
	m_rotation.AddRotationDegY(deltaTime * m_rotateSpeed);


	// コインの位置にSEを追従させる
	m_coinSE->SetPosition(GetPosition());
}

void Coin::PlayCollectEffects()
{
	// コイン取得音再生
	auto* se = NewGO<nsK2EngineLow::SoundSource>(0, "CoinGetSE");
	se->Init(static_cast<int>(SoundID::enCoinGetSE), false);
	se->Play(false);

	// コイン取得エフェクト再生
	m_collectEffect = NewGO<EffectEmitter>(0);
	m_collectEffect->Init(enCoinGet);
	m_collectEffect->SetPosition(m_position);
	m_collectEffect->SetScale(Vector3::One * 10);
	m_collectEffect->SetRotation(Quaternion::Identity);
	m_collectEffect->Play();

}
