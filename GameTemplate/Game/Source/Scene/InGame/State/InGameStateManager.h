#pragma once
#include"Source/Scene/SceneStateContext.h"
#include"Source/Scene/InGame/State/InGameStateType.h"

class IInGameState;
class InGameStateManager :public IGameObject
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
	bool IsEnd()const
	{
		return m_isEnd;
	}
	void Render(RenderContext& rc) override;

private:
	IInGameState* m_currentState = nullptr;
	InGameContext m_inGameContext;
	InGameStateType m_nextStateType;
	bool m_isEnd = false;

};

