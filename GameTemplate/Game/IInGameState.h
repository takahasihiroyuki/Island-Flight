#pragma once
#include"SceneStateContext.h"
#include"InGameStateType.h"

class IInGameState
{
public:
	IInGameState() {};
	virtual ~IInGameState() {};
	virtual void Enter(InGameContext& context)final
	{
		m_context = &context;
		OnEnter();
	}
	virtual void OnEnter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual bool RequestChangeState(InGameStateType& type)=0;

protected:
	InGameContext* m_context = nullptr;
};

