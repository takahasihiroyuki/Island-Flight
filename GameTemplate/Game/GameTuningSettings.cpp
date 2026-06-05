#include "stdafx.h"
#include "GameTuningSettings.h"

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

		return settings;
	}
}