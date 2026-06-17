#pragma once
class Timer;
class ScoreManager;
class AdddTimePopupUI;
struct BonusItemEffectContext
{
	Timer* gameTimer = nullptr;
	ScoreManager* scoreManager = nullptr;
	class AddTimePopupUI* addTimePopupUI = nullptr;

};