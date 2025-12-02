#pragma once
#include"IScene.h"
#include "TitleUI.h"

class Aircraft;
class TitleScene :public IScene
{
public:
	TitleScene() {};
	~TitleScene();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	bool RequestChangeScene(SceneType& type) override;
	void Enter();
	void Exit();


private:
	std::unique_ptr<TitleUI> m_titleUI;
	SoundSource* m_titleBGM = nullptr;
	Aircraft* m_aircraft = nullptr;
	SkyCube* m_skyCube = nullptr;
	Ocean* m_ocean = nullptr;
	float m_elapsedTime = 0.0f;
};

