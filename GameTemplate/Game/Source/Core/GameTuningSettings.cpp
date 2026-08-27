#include "Source/stdafx.h"
#include "Source/Core/GameTuningSettings.h"

namespace GameTuning
{
	GameTuningSettings CreateDefaultGameTuningSettings()
	{
		GameTuningSettings settings;

		// コンボシステムのデフォルト設定
		{
			settings.scoreManager.combo.duration = 6.0f;
			settings.scoreManager.combo.maxMultiplier = 5.0f;
		}

		// ボーナスアイテムマネージャーのデフォルト設定
		{
			settings.bonusItemManager.collectRadius = 120.0f;
			settings.bonusItemManager.addTime.addSeconds = 10.0f;
			settings.bonusItemManager.scoreBoost.multiplier = 2.0f;
			settings.bonusItemManager.scoreBoost.duration = 10.0f;
		}

		return settings;
	}
}