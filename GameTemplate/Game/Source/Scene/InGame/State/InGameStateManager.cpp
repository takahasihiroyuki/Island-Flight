#include "Source/stdafx.h"
#include "Source/Scene/InGame/State/InGameStateManager.h"
#include "Source/Scene/InGame/State/ResultState.h"
#include "Source/Scene/InGame/State/IInGameState.h"
#include "Source/Scene/InGame/State/GamePlayState.h"

namespace {
	InGameStateType INIT_STATE = InGameStateType::enPlay;
}

InGameStateManager::InGameStateManager()
{
}

InGameStateManager::~InGameStateManager()
{
	if (m_currentState) {
		delete m_currentState;
		m_currentState = nullptr;
	}
}

bool InGameStateManager::Start()
{
	Change(INIT_STATE);
	return true;
}

void InGameStateManager::Update()
{
	m_currentState->Update();
	if (m_currentState->RequestChangeState(m_nextStateType))
	{
		Change(m_nextStateType);
	}
}

void InGameStateManager::Change(InGameStateType type)
{

	if (m_currentState) {
		m_currentState->Exit();
		delete m_currentState;
		m_currentState = nullptr;
	}

	//タイプがエンドならこのクラスを終わらせるフラグを立てるだけ
	if (type == InGameStateType::enEnd)
	{
		m_isEnd = true;
		return;
	}

	switch (type) {
	case InGameStateType::enPlay:
		m_currentState = new GamePlayState();
		break;
	case InGameStateType::enResult:
		m_currentState = new ResultState();
		break;
	}


	if (m_currentState) {
		m_currentState->Enter(m_inGameContext);
	}
}

void InGameStateManager::Render(RenderContext& rc)
{
	if (m_currentState) {
		m_currentState->Render(rc);
	}
}
