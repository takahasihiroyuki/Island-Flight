#pragma once
#include"IInGameState.h"
class ResoltState :public IInGameState
{
public:
	ResoltState();
	~ResoltState();
	void OnEnter()override;
	void Update()override;
	void Exit()override;
	bool RequestChangeState(InGameStateType& type)override;
};

