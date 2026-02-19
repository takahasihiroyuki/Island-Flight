#include "stdafx.h"
#include "CoinCounterUI.h"
#include "CoinManager.h"

namespace
{
	const Vector3 SPRITE_BASE_POS = Vector3(-500.0f, 360.0f, 0.0f);
	constexpr float SPACING_X = 50;//数字の隙間の長さ（ｘ）
	const Vector3 ONES_SPLITE_POS = Vector3(SPRITE_BASE_POS.x, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 TENS_SPLITE_POS = Vector3(SPRITE_BASE_POS.x - SPACING_X, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 HUNDREDS_SPLITE_POS = Vector3(SPRITE_BASE_POS.x - SPACING_X * 2, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 COINUI_SPLITE_POS = Vector3(SPRITE_BASE_POS.x - SPACING_X * 4, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 SPRITE_SCALE = Vector3(0.7f, 0.7f, 0.7f);
}


CoinCounterUI::CoinCounterUI()
{
	m_animDurationClose = 1.0f;
	m_animDurationOpen = 1.0f;
}

CoinCounterUI::~CoinCounterUI()
{
	DeleteGO(m_coinManager);
}

void CoinCounterUI::OnUpdate()
{
	m_displayNumber = m_coinManager->GetCoinCount();

	m_displayOnesNum = m_displayNumber % 10;
	m_displayTensNum = (m_displayNumber / 10) % 10;
	m_displayHundredsNum = (m_displayNumber / 100) % 10;

	// アニメーションのオフセットを適用
	ApplyOffset();
}

void CoinCounterUI::Render(RenderContext& rc)
{
	m_coinUI.Draw(rc);
	OnesSprite[m_displayOnesNum].Draw(rc);
	TensSprite[m_displayTensNum].Draw(rc);
	HundredsSprite[m_displayHundredsNum].Draw(rc);
}

void CoinCounterUI::Init()
{
	m_spritePaths[0] = "Assets/UI/Numbers/timelimit_zero.DDS";
	m_spritePaths[1] = "Assets/UI/Numbers/timelimit_one.DDS";
	m_spritePaths[2] = "Assets/UI/Numbers/timelimit_two.DDS";
	m_spritePaths[3] = "Assets/UI/Numbers/timelimit_three.DDS";
	m_spritePaths[4] = "Assets/UI/Numbers/timelimit_four.DDS";
	m_spritePaths[5] = "Assets/UI/Numbers/timelimit_five.DDS";
	m_spritePaths[6] = "Assets/UI/Numbers/timelimit_six.DDS";
	m_spritePaths[7] = "Assets/UI/Numbers/timelimit_seven.DDS";
	m_spritePaths[8] = "Assets/UI/Numbers/timelimit_eight.DDS";
	m_spritePaths[9] = "Assets/UI/Numbers/timelimit_nine.DDS";
	m_coinUI.Init("Assets/UI/CoinCounter/coin.DDS", 200, 200);
	m_coinUI.SetPosition(COINUI_SPLITE_POS);
	m_coinUI.Update();
	for (int i = 0; i < 10; i++)
	{
		OnesSprite[i].Init(m_spritePaths[i], 100, 100);
		OnesSprite[i].SetPosition(ONES_SPLITE_POS);
		OnesSprite[i].SetScale(SPRITE_SCALE);
		OnesSprite[i].Update();

		TensSprite[i].Init(m_spritePaths[i], 100, 100);
		TensSprite[i].SetPosition(TENS_SPLITE_POS);
		TensSprite[i].SetScale(SPRITE_SCALE);
		TensSprite[i].Update();

		HundredsSprite[i].Init(m_spritePaths[i], 100, 100);
		HundredsSprite[i].SetPosition(HUNDREDS_SPLITE_POS);
		HundredsSprite[i].SetScale(SPRITE_SCALE);
		HundredsSprite[i].Update();

	}

	m_coinManager = FindGO<CoinManager>("coinManager");

}

void CoinCounterUI::OnOpen()
{
}

void CoinCounterUI::OnClose()
{
}

void CoinCounterUI::OnOpenAnimUpdate(float t)
{
	float positionT = t;

	// 非表示アニメーション
	m_uiOpenCloseAnimOffset.Lerp(positionT, m_uiHiddenPos, SPRITE_BASE_POS);
	m_uiOpenCloseAnimOffset -= SPRITE_BASE_POS;

}

void CoinCounterUI::OnCloseAnimUpdate(float t)
{
	float positionT = t;

	// 非表示アニメーション
	m_uiOpenCloseAnimOffset.Lerp(positionT, SPRITE_BASE_POS, m_uiHiddenPos);
	m_uiOpenCloseAnimOffset -= SPRITE_BASE_POS;

}

void CoinCounterUI::ApplyOffset()
{
	const Vector3 offset = m_uiOpenCloseAnimOffset;

	m_coinUI.SetPosition(COINUI_SPLITE_POS + offset);
	m_coinUI.Update();

	for (int i = 0; i < 10; i++)
	{
		OnesSprite[i].SetPosition(ONES_SPLITE_POS + offset);
		OnesSprite[i].Update();

		TensSprite[i].SetPosition(TENS_SPLITE_POS + offset);
		TensSprite[i].Update();

		HundredsSprite[i].SetPosition(HUNDREDS_SPLITE_POS + offset);
		HundredsSprite[i].Update();
	}

}
