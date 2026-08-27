#pragma once
#include"Source/Scene/InGame/State/IInGameState.h"

enum class ResultPhase
{
	WaitingInput,   // 普通のリザルト画面
	Outro,          // A押した後の演出（フェードアウト）
	ToNextScene,    // 次のシーンへ移行
};

class ResultUI;
class ResultState :public IInGameState
{
public:
	ResultState();
	~ResultState();
	void OnEnter()override;
	void Update()override;
	void Exit()override;
	bool RequestChangeState(InGameStateType& type)override;


private:
	std::unique_ptr<ResultUI> m_resultUI;
	SoundSource* m_resultEnterSE = nullptr;
	ResultPhase m_phase = ResultPhase::WaitingInput;
	float m_outroFinishTime = 3.0f;
	float m_outroElapsedTime = 0.0f;
};
