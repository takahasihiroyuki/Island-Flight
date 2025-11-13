#include "stdafx.h"
#include "InGameStateManager.h"
#include "ResoltState.h"
#include "IInGameState.h"
#include "GamePlayState.h"

namespace {
	InGameStateType INIT_STATE = InGameStateType::enPlay;
}

InGameStateManager::InGameStateManager()
{
	int debug = 0;

}

InGameStateManager::~InGameStateManager()
{
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
	}

	switch (type) {
	case InGameStateType::enPlay:
		m_currentState = new GamePlayState();
		break;
	case InGameStateType::enResolt:
		m_currentState = new ResoltState();
		break;
	}

	if (m_currentState) {
		m_currentState->Enter(m_inGameContext);
	}
}
