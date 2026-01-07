#pragma once
#include"IInGameState.h"

enum class GamePlayPhase
{
	GamePlay,
	Outro
};


class Timer;
class TimerUI;
class CoinDirectionArrowUI;
class CoinCounterUI;
class GamePlayState :public IInGameState
{
public:
	GamePlayState();
	~GamePlayState();
	void OnEnter()override;
	void Update()override;
	void Exit()override;
	bool RequestChangeState(InGameStateType& type)override;


private:
	Timer* m_timer = nullptr;
	std::unique_ptr<TimerUI> m_timeUI;
	std::unique_ptr<CoinDirectionArrowUI> m_coinArrowUI = nullptr;
	std::unique_ptr<CoinCounterUI> m_coinCounterUI = nullptr;
	SoundSource* m_gamePlayBGM = nullptr;
	GamePlayPhase m_phase = GamePlayPhase::GamePlay;
	bool m_isChangeToResult = false;
	float m_outroFinishTime = 1.0f;
};

