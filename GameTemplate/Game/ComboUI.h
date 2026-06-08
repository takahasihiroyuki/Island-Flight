#pragma once
#include"UIScreen.h"
class ComboUI :public UIScreen
{
	public:
	ComboUI() {};
	~ComboUI() {};
	void OnUpdate()override;
	void Render(RenderContext& rc)override;
	void Init();
	void OnOpen() {};
	void OnClose() {};

private:
	void ApplyOffset();
};

