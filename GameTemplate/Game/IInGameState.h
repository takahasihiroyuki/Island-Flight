#pragma once
#include"SceneStateContext.h"

class IInGameState
{
public:
	IInGameState() {};
	virtual ~IInGameState() {};
	virtual void Enter(InGameContext& context)final
	{
		m_Context = &context;
		OnEnter();
	}
	virtual void OnEnter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual void Init() = 0;

protected:
	InGameContext* m_Context = nullptr;
};

