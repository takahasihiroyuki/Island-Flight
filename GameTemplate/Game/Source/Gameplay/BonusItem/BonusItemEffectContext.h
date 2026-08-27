#pragma once
class Timer;
class ScoreManager;
class AddTimePopupUI;
struct BonusItemEffectContext
{
	Timer* gameTimer = nullptr;
	ScoreManager* scoreManager = nullptr;
	AddTimePopupUI* addTimePopupUI = nullptr;

};