#include "stdafx.h"
#include "ScoreBoostItem.h"
#include "ScoreManager.h"

void ScoreBoostItem::ApplyEffect(const BonusItemEffectContext& context)
{
	if (context.scoreManager == nullptr)
	{
		return;
	}

	context.scoreManager->StartScoreBoost(
		m_multiplier,
		m_duration
	);
}
