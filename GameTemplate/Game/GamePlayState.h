#pragma once
#include"IInGameState.h"
class Timer;
class TimerUI;
class GamePlayState :public IInGameState
{
public:
	GamePlayState();
	~GamePlayState();
	void OnEnter()override;
	void Update()override;
	void Exit()override;
	bool RequestChangeState(InGameStateType& type)override;


private:
	Timer* m_timer = nullptr;
	std::unique_ptr<TimerUI> m_timeUI;
};

