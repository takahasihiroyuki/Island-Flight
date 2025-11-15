#include "stdafx.h"
#include "TimerUI.h"
#include "Timer.h"

namespace
{
	const Vector2 SPRITE_BASE_POS = Vector2(10.0f, 10.0f);
	constexpr float SPACING_X = 5;//数字の隙間の長さ（ｘ）
	const Vector2 ONES_SPLITE_POS = Vector2(SPRITE_BASE_POS.x, SPRITE_BASE_POS.y);
	const Vector2 TENS_SPLITE_POS = Vector2(SPRITE_BASE_POS.x - SPACING_X, SPRITE_BASE_POS.y);
	const Vector2 HUNDREDS_SPLITE_POS = Vector2(SPRITE_BASE_POS.x - SPACING_X * 2, SPRITE_BASE_POS.y);

}

void TimerUI::Update()
{
	//時間を適用させる。
	ApplyTime(m_timer->GetElapsedTime());

	//前フレームと数字が違ったらスプライトを変える。
	if (m_displayOnesNum != m_prevDisplayOnesNum) {
		OnesSprite.Init(m_spritePaths[m_displayOnesNum], ONES_SPLITE_POS.x, ONES_SPLITE_POS.y);
	}
	if (m_displayOnesNum != m_prevDisplayTensNum) {
		TensSprite.Init(m_spritePaths[m_displayTensNum], TENS_SPLITE_POS.x, TENS_SPLITE_POS.y);
	}
	if (m_displayOnesNum != m_prevDisplayTensNum) {
		HundredsSprite.Init(m_spritePaths[m_displayHundredsNum], HUNDREDS_SPLITE_POS.x, HUNDREDS_SPLITE_POS.y);
	}
}

void TimerUI::Render(RenderContext& rc)
{
	OnesSprite.Draw(rc);
	TensSprite.Draw(rc);
	HundredsSprite.Draw(rc);
}

void TimerUI::Init(Timer* timer)
{
	SetTimer(timer);

	//後で10まで初期化する
	m_spritePaths[0] = "Assets/UI/Numbers/timelimit_ zero.DDS";
	m_spritePaths[1] = "Assets/UI/Numbers/timelimit_ one.DDS";
	m_spritePaths[2] = "Assets/UI/Numbers/timelimit_ two.DDS";
	m_spritePaths[3] = "Assets/UI/Numbers/timelimit_ three.DDS";
	m_spritePaths[4] = "Assets/UI/Numbers/timelimit_ four.DDS";
	m_spritePaths[5] = "Assets/UI/Numbers/timelimit_ five.DDS";
	m_spritePaths[6] = "Assets/UI/Numbers/timelimit_ six.DDS";
	m_spritePaths[7] = "Assets/UI/Numbers/timelimit_ seven.DDS";
	m_spritePaths[8] = "Assets/UI/Numbers/timelimit_ eight.DDS";
	m_spritePaths[9] = "Assets/UI/Numbers/timelimit_ nine.DDS";


	OnesSprite.Init(m_spritePaths[0], ONES_SPLITE_POS.x, ONES_SPLITE_POS.y);
	TensSprite.Init(m_spritePaths[0], TENS_SPLITE_POS.x, TENS_SPLITE_POS.y);
	HundredsSprite.Init(m_spritePaths[0], HUNDREDS_SPLITE_POS.x, HUNDREDS_SPLITE_POS.y);
}

void TimerUI::ApplyTime(float elapsedTime)
{
	m_prevDisplayOnesNum = m_displayOnesNum;
	m_prevDisplayTensNum = m_displayTensNum;
	m_prevDisplayHundredsNum = m_displayHundredsNum;

	//小数部分切り上げ
	m_displayNumber = static_cast<int>(ceilf(elapsedTime));

	//小数部分を補完率として使う
	m_t = m_displayNumber - elapsedTime;

	m_displayOnesNum = m_displayNumber % 10;
	m_displayTensNum = (m_displayNumber / 10) % 10;
	m_displayHundredsNum = (m_displayNumber / 100) % 10;

}
