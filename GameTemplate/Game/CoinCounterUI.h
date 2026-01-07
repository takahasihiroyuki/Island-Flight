#pragma once
#include"UIScreen.h"
class CoinManager;
class CoinCounterUI :public UIScreen
{
public:
	CoinCounterUI() {};
	~CoinCounterUI();
	void Update()override;
	void Render(RenderContext& rc)override;
	void Init();
	void Open();
	void Close();

private:


private:
	CoinManager* m_coinManager = nullptr;
	std::array< SpriteRender, 10> OnesSprite;
	std::array< SpriteRender, 10> TensSprite;
	std::array< SpriteRender, 10> HundredsSprite;
	int m_displayOnesNum = 0;
	int m_displayTensNum = 0;
	int m_displayHundredsNum = 0;
	int m_displayThousandsNum = 0;
	int m_displayNumber = 0;//•\¦‚·‚é”š
	std::array<const char*, 10> m_spritePaths;
	SpriteRender m_coinUI;
};


