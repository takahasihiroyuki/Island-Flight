#include "stdafx.h"
#include "AddTimeItem.h"
#include "Timer.h"

void AddTimeItem::ApplyEffect(
	const BonusItemEffectContext& context)
{
	if (context.gameTimer == nullptr)
	{
		return;
	}

	context.gameTimer->AddLimitTime(m_addSeconds);
}