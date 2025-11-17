#include "stdafx.h"
#include "ResultUI.h"
#include "ScoreManager.h"
#include "UIManager.h"

namespace
{
	const Vector3 SPRITE_BASE_POS = Vector3(0.0f, 0.0f, 0.0f);
	constexpr float SPACING_X = 75;//”Žš‚ÌŒ„ŠÔ‚Ì’·‚³i‚˜j
	const Vector3 ONES_SPLITE_POS = Vector3(SPRITE_BASE_POS.x, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 TENS_SPLITE_POS = Vector3(SPRITE_BASE_POS.x - SPACING_X, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 HUNDREDS_SPLITE_POS = Vector3(SPRITE_BASE_POS.x - SPACING_X * 2, SPRITE_BASE_POS.y, 0.0f);

}


ResultUI::~ResultUI()
{
}

void ResultUI::Update()
{
	ApplyScore(m_scoreManager->GetScore());
}

void ResultUI::Render(RenderContext& rc)
{
	OnesSprite[m_displayOnesNum].Draw(rc);
	TensSprite[m_displayTensNum].Draw(rc);
	HundredsSprite[m_displayHundredsNum].Draw(rc);
}

void ResultUI::Init()
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

	for (int i = 0; i < 10; i++)
	{
		OnesSprite[i].Init(m_spritePaths[i], 100, 100);
		OnesSprite[i].SetPosition(ONES_SPLITE_POS);
		OnesSprite[i].Update();
	}

	for (int i = 0; i < 10; i++)
	{
		TensSprite[i].Init(m_spritePaths[i], 100, 100);
		TensSprite[i].SetPosition(TENS_SPLITE_POS);
		TensSprite[i].Update();
	}

	for (int i = 0; i < 10; i++)
	{
		HundredsSprite[i].Init(m_spritePaths[i], 100, 100);
		HundredsSprite[i].SetPosition(HUNDREDS_SPLITE_POS);
		HundredsSprite[i].Update();

	}

}

void ResultUI::Close()
{
	UIManager::GetInstance().UnregisterScreen("ResultUI");
}

void ResultUI::ApplyScore(float score)
{
	//¬”•”•ªØ‚èã‚°
	m_displayNumber = static_cast<int>(ceilf(score));


	m_displayOnesNum = m_displayNumber % 10;
	m_displayTensNum = (m_displayNumber / 10) % 10;
	m_displayHundredsNum = (m_displayNumber / 100) % 10;

}
