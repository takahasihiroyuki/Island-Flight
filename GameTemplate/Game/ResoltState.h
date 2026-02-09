#pragma once
#include"IInGameState.h"

enum class ResoltPhase
{
	WaitingInput,   // 普通のリザルト画面
	Outro,          // A押した後の演出（フェードアウト）
	ToNextScene,    // 次のシーンへ移行
};

class ResultUI;
class ResoltState :public IInGameState
{
public:
	ResoltState();
	~ResoltState();
	void OnEnter()override;
	void Update()override;
	void Exit()override;
	bool RequestChangeState(InGameStateType& type)override;


private:
	std::unique_ptr<ResultUI> m_resultUI;
	SoundSource* m_resultEnterSE = nullptr;
	ResoltPhase m_phase = ResoltPhase::WaitingInput;
	float m_outroFinishTime = 3.0f;
	float m_outroElapsedTime = 0.0f;
};
