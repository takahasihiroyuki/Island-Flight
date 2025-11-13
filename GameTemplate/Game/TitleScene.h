#pragma once
#include"IScene.h"

class TitleScene :public IScene
{
public:
	TitleScene() {};
	~TitleScene() {};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	bool RequestChangeScene(SceneType& type) override;
	void Enter() {};
	void Exit() {};
};

