#pragma once
#include"SceneStateContext.h"
#include"InGameStateType.h"

class IInGameState;
class InGameStateManager:public IGameObject
{
public:
	InGameStateManager();
	~InGameStateManager();
	bool Start();
	void Update();
	void Change(InGameStateType type);
	void SetContext(InGameContext& inGameContext) 
	{
		m_inGameContext = inGameContext;
	}


private:
	IInGameState* m_currentState = nullptr;
	InGameContext m_inGameContext;
	InGameStateType m_nextStateType;

};

