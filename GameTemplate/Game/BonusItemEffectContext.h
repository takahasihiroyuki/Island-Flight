#pragma once
class Timer;
class ScoreManager;
struct BonusItemEffectContext
{
	Timer* gameTimer = nullptr;
	ScoreManager* scoreManager = nullptr;
};