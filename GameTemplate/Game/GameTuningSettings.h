#pragma once

namespace GameTuning
{
	struct ComboConfig
	{
		float duration = 3.0f;			// コンボが継続する時間（秒）
		float maxMultiplier = 5.0f;     // 最大スコア倍率
	};

	struct ScoreManagerConfig
	{
		ComboConfig combo;			// コンボシステムの設定
	};

	struct GameTuningSettings
	{
		ScoreManagerConfig scoreManager;	// スコアマネージャーの設定
	};

	GameTuningSettings CreateDefaultGameTuningSettings();
}
