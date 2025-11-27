#pragma once
#include"IInGameState.h"
class ResultUI;
class ResoltState :public IInGameState
{
public:
	ResoltState();
	~ResoltState();
	void OnEnter()override;
	void Update()override;
	void Exit()override;
	bool RequestChangeState(InGameStateType& type)override;


private:
	std::unique_ptr<ResultUI> m_resultUI;
	SoundSource* m_resultEnterSE = nullptr;
};
