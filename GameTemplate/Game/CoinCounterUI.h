#pragma once
#include"UIScreen.h"
class CoinManager;
class CoinCounterUI :public UIScreen
{
public:
	CoinCounterUI();
	~CoinCounterUI();
	void OnUpdate()override;
	void Render(RenderContext& rc)override;
	void Init();
	void OnOpen();
	void OnClose();
	void OnOpenAnimUpdate(float t)override;
	void OnCloseAnimUpdate(float t)override;

private:
	void ApplyOffset();

private:
	CoinManager* m_coinManager = nullptr;
	std::array< SpriteRender, 10> OnesSprite;
	std::array< SpriteRender, 10> TensSprite;
	std::array< SpriteRender, 10> HundredsSprite;
	int m_displayOnesNum = 0;
	int m_displayTensNum = 0;
	int m_displayHundredsNum = 0;
	int m_displayThousandsNum = 0;
	int m_displayNumber = 0;//表示する数字
	std::array<const char*, 10> m_spritePaths;
	SpriteRender m_coinUI;

	Vector3 m_uiHiddenPos = Vector3(-500.0f, 560.0f, 0.0f);		//非表示時のポジション

	Vector3 m_uiOpenCloseAnimOffset; // 表示、非表示アニメーション用の位置オフセット

};


