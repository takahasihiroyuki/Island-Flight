#pragma once
#include"IInGameState.h"
class GamePlayState :public IInGameState
{
public:
	GamePlayState();
	~GamePlayState();
	void OnEnter() {};
	void Update();
	void Exit();
	void Init();
};

