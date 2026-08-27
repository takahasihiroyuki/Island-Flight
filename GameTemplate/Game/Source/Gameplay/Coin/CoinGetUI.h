#pragma once
#include"Source/UI/UIScreen.h"
class CoinGetUI :public UIScreen
{
public:
	CoinGetUI() {};
	~CoinGetUI() {};
	void OnUpdate()override;
	void Render(RenderContext& rc)override;
	void Init();
	void OnOpen() {};
	void OnClose() {};


private:


private:
	SpriteRender m_coinGetUI;
};

