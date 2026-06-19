#include "stdafx.h"
#include "ScoreBoostItem.h"
#include "ScoreManager.h"

namespace {
	Vector3 EFFECT_SIZE = Vector3{ 10,10,10 };
}

void ScoreBoostItem::ApplyEffect(const BonusItemEffectContext& context)
{
	if (context.scoreManager == nullptr)
	{
		return;
	}

	//取ったときのSE
	{
		auto* se = NewGO<SoundSource>(0, "ScoreBoostItemGetSE");
		se->Init(static_cast<int>(SoundID::enScoreBoostItemGetSE), false);
		se->Play(false);
	}

	//取ったときのエフェクト
	{
		EffectEngine::GetInstance()->ResistEffect(
			enScoreBoostPickup,
			effectPath[enScoreBoostPickup]
		);

		auto* effect = NewGO<EffectEmitter>(0);
		effect->Init(enScoreBoostPickup);
		effect->SetPosition(GetPosition());
		effect->SetRotation(Quaternion::Identity);
		effect->SetScale(EFFECT_SIZE);
		effect->Play();
	}

	context.scoreManager->StartScoreBoost(
		m_multiplier,
		m_duration
	);
}
