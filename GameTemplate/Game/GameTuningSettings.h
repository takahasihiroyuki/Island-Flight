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

	struct AddTimeItemConfig
	{
		int itemActiveCount = 2;	//ゲーム中にアクティブにする数
		float addSeconds = 10.0f;
	};

	struct ScoreBoostItemConfig
	{
		int itemActiveCount = 2;	//ゲーム中にアクティブにする数
		float multiplier = 2.0f;
		float duration = 10.0f;
	};

	struct BonusItemManagerConfig
	{
		float collectRadius = 3000.0f;

		AddTimeItemConfig addTime;
		ScoreBoostItemConfig scoreBoost;
	};

	/// <summary>
	/// ゲーム全体のチューニング設定
	/// </summary>
	struct GameTuningSettings
	{
		ScoreManagerConfig scoreManager;	// スコアマネージャーの設定
		BonusItemManagerConfig bonusItemManager; // ボーナスアイテムマネージャーの設定
	};

	GameTuningSettings CreateDefaultGameTuningSettings();
}
