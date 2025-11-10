#include "stdafx.h"
#include "InGameStateManager.h"
#include "IInGameState.h"
#include "GamePlayState.h"

namespace {
	InGameStateType INIT_STATE = InGameStateType::enPlay;
}

InGameStateManager::InGameStateManager()
{
}

InGameStateManager::~InGameStateManager()
{
}

bool InGameStateManager::Start()
{
	Change(INIT_STATE);
	return false;
}

void InGameStateManager::Update()
{
}

void InGameStateManager::Change(InGameStateType type)
{
	if (m_currentState) {
		m_currentState->Exit();
		delete m_currentState;
	}

	switch (type) {
	case InGameStateType::enPlay:
		m_currentState =new GamePlayState;
		m_currentState->Init();
		break;
	}

	if (m_currentState) {
		m_currentState->Enter(m_inGameContext);
	}
}
