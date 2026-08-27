#pragma once
#include"Source/Scene/SceneStateContext.h"
#include"Source/Scene/InGame/State/InGameStateType.h"

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
	virtual void Render(RenderContext& rc) {};

protected:
	InGameContext* m_context = nullptr;
};

