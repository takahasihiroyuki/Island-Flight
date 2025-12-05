#pragma once
#include"UIScreen.h"
class CoinGetUI :public UIScreen
{
public:
	CoinGetUI() {};
	~CoinGetUI() {};
	void Update()override;
	void Render(RenderContext& rc)override;
	void Init();
	void Open();
	void Close();


private:


private:
	SpriteRender m_coinGetUI;
};

