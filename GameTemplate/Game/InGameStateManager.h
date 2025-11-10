#pragma once
#include"SceneStateContext.h"

enum class InGameStateType
{
	enPlay,
}; 

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
};

