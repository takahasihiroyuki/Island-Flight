#pragma once
#include"Source/Scene/InGame/State/IInGameState.h"
#include "Source/Gameplay/Assist/PlayerAssistSystem.h"

enum class GamePlayPhase
{
	GamePlay,
	Outro
};


class Timer;
class Aircraft;
class ScorePopupScreen;
class ComboCounterScreen;
class BonusItemManager;
class GamePlayState :public IInGameState
{
public:
	GamePlayState();
	~GamePlayState();
	void OnEnter()override;
	void Update()override;
	void Exit()override;
	bool RequestChangeState(InGameStateType& type)override;
	void PlayerInput();


private:
	Timer* m_timer = nullptr;
	ScorePopupScreen* m_scorePopupScreen = nullptr;
	ComboCounterScreen* m_comboCounterScreen = nullptr;

	SoundSource* m_gamePlayBGM = nullptr;
	GamePlayPhase m_phase = GamePlayPhase::GamePlay;
	bool m_isChangeToResult = false;
	float m_outroFinishTime = 1.0f;
	PlayerAssistSystem m_playerAssistSystem;
};