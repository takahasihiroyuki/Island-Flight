#include "Source/stdafx.h"
#include "Source/Gameplay/BonusItem/AddTimeItem.h"
#include "Source/Gameplay/Time/Timer.h"
#include "Source/Gameplay/BonusItem/AddTimePopupUI.h"

namespace {
	Vector3 EFFECT_SIZE = Vector3{ 10,10,10 };
}

void AddTimeItem::ApplyEffect(
	const BonusItemEffectContext& context)
{
	if (context.gameTimer == nullptr)
	{
		return;
	}

	//取ったときのSE
	{
		auto* se = NewGO<SoundSource>(0, "AddTimeLimitItemGetSE");
		se->Init(static_cast<int>(SoundID::enAddTimeLimitItemGetSE), false);
		se->Play(false);
	}

	//取ったときのエフェクト
	{
		EffectEngine::GetInstance()->RegisterEffect(
			enAddLimitCollectEffect,
			effectPath[enAddLimitCollectEffect]
		);

		auto* effect = NewGO<EffectEmitter>(0);
		effect->Init(enAddLimitCollectEffect);
		effect->SetPosition(GetPosition());
		effect->SetRotation(Quaternion::Identity);
		effect->SetScale(EFFECT_SIZE);
		effect->Play();
	}


	context.gameTimer->AddLimitTime(m_addSeconds);

	if (context.addTimePopupUI != nullptr)
	{
		context.addTimePopupUI->Play(m_addSeconds);
	}

}