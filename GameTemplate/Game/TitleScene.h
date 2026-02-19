#pragma once
#include"IScene.h"
#include "TitleUI.h"

/// <summary>
/// タイトルのフェーズ
/// </summary>
enum class TitlePhase
{
	WaitingInput,   // 普通のタイトル（カメラ追従してもいい）
	Outro,          // A押した後の演出（カメラ固定・飛行機が遠ざかる）
	ToNextScene,    // 演出完了
};

class Aircraft;
class TitleScene :public IScene
{
public:
	TitleScene() {};
	~TitleScene();

	bool Start() override;
	void Update() override;
	void WaitingInputPhaseUpdate();
	void OutroPhaseUpdate();
	void Render(RenderContext& rc) override;
	bool RequestChangeScene(SceneType& type) override;
	void Enter();
	void Exit();


private:
	void  CameraTargetOffsetUpdate();


private:
	std::unique_ptr<TitleUI> m_titleUI;
	SoundSource* m_titleBGM = nullptr;
	Aircraft* m_aircraft = nullptr;
	SkyCube* m_skyCube = nullptr;
	Ocean* m_ocean = nullptr;
	float m_elapsedTime = 0.0f;
	TitlePhase m_phase = TitlePhase::WaitingInput;
	float m_outroFinishTime = 3.0f;
	ModelRender m_groundModel;
	Vector3 CameraTargetPosOffset;
};

