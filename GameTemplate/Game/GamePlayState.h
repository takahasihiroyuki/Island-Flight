#pragma once
#include"IInGameState.h"
class Timer;
class GamePlayState :public IInGameState
{
public:
	GamePlayState();
	~GamePlayState();
	void OnEnter();
	void Update();
	void Exit();
	void Init();
	bool RequestChangeState(InGameStateType& type);


private:
	Timer* m_timer=nullptr;
};

