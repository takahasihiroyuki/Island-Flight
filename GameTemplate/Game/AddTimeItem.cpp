#include "stdafx.h"
#include "AddTimeItem.h"
#include "Timer.h"
#include "AddTimePopupUI.h"

void AddTimeItem::ApplyEffect(
	const BonusItemEffectContext& context)
{
	if (context.gameTimer == nullptr)
	{
		return;
	}

	context.gameTimer->AddLimitTime(m_addSeconds);

	if (context.addTimePopupUI != nullptr)
	{
		context.addTimePopupUI->Play(m_addSeconds);
	}

}