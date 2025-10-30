#pragma once
#include"IScene.h"

class TitleScene :public IScene
{
public:
	TitleScene() {};
	~TitleScene() {};

	bool Start() override;
	void OnUpdate() override;
	void Render(RenderContext& rc) override;
	void CheckChangeScene() override;
};

