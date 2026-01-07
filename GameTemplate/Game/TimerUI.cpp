#include "stdafx.h"
#include "TimerUI.h"
#include "Timer.h"
#include "UIManager.h"

namespace
{
	const Vector3 SPRITE_BASE_POS = Vector3(650.0f, 360.0f, 0.0f);
	constexpr float SPACING_X = 75.0f; // 数字の隙間（X方向）

	// 秒の一の位（右端）
	const Vector3 SEC_ONES_POS = Vector3(
		SPRITE_BASE_POS.x,
		SPRITE_BASE_POS.y,
		0.0f
	);

	// 秒の十の位
	const Vector3 SEC_TENS_POS = Vector3(
		SPRITE_BASE_POS.x - SPACING_X,
		SPRITE_BASE_POS.y,
		0.0f
	);

	// コロン（：）
	const Vector3 COLON_POS = Vector3(
		SPRITE_BASE_POS.x - SPACING_X * 2.0f,
		SPRITE_BASE_POS.y,
		0.0f
	);

	// 分の一の位
	const Vector3 MIN_ONES_POS = Vector3(
		SPRITE_BASE_POS.x - SPACING_X * 3.0f,
		SPRITE_BASE_POS.y,
		0.0f
	);

	// 分の十の位
	const Vector3 MIN_TENS_POS = Vector3(
		SPRITE_BASE_POS.x - SPACING_X * 4.0f,
		SPRITE_BASE_POS.y,
		0.0f
	);

	const Vector3 TIMER_BACK_POS = Vector3(
		SPRITE_BASE_POS.x - SPACING_X * 2.0f,
		SPRITE_BASE_POS.y,
		0.0f
	);
}

void TimerUI::Update()
{
	//時間を適用させる。
	ApplyTime(m_timer->GetRemainingTime());
}

void TimerUI::Render(RenderContext& rc)
{
	//m_timerBackSprite.Draw(rc);
	m_minOnesSprite[m_displayMinOnesNum].Draw(rc);
	m_minTensSprite[m_displayMinTensNum].Draw(rc);
	m_secOnesSprite[m_displaySecOnesNum].Draw(rc);
	m_secTensSprite[m_displaySecTensNum].Draw(rc);

	m_colonSprite.Draw(rc);
}

void TimerUI::Init(Timer* timer)
{
	SetTimer(timer);

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
		m_minOnesSprite[i].Init(m_spritePaths[i], 100, 100);
		m_minOnesSprite[i].SetPosition(MIN_ONES_POS);
		m_minOnesSprite[i].Update();
	}

	for (int i = 0; i < 10; i++)
	{
		m_minTensSprite[i].Init(m_spritePaths[i], 100, 100);
		m_minTensSprite[i].SetPosition(MIN_TENS_POS);
		m_minTensSprite[i].Update();
	}

	for (int i = 0; i < 10; i++)
	{
		m_secOnesSprite[i].Init(m_spritePaths[i], 100, 100);
		m_secOnesSprite[i].SetPosition(SEC_ONES_POS);
		m_secOnesSprite[i].Update();
	}

	for (int i = 0; i < 10; i++)
	{
		m_secTensSprite[i].Init(m_spritePaths[i], 100, 100);
		m_secTensSprite[i].SetPosition(SEC_TENS_POS);
		m_secTensSprite[i].Update();
	}

	m_colonSprite.Init("Assets/UI/Timer/colon.DDS", 100, 100);
	m_colonSprite.SetPosition(COLON_POS);
	m_colonSprite.Update();

	m_timerBackSprite.Init("Assets/UI/Timer/TimerBack.DDS", 600, 200);
	m_timerBackSprite.SetPosition(COLON_POS);
	m_timerBackSprite.Update();

}

void TimerUI::Open()
{
}

void TimerUI::Close()
{
	UIManager::GetInstance().UnregisterScreen(GetName());
}

void TimerUI::ApplyTime(float elapsedTime)
{
	// 小数部分切り上げ（残り秒を表示する想定）
	m_totalDisplaySec = static_cast<int>(ceilf(elapsedTime));

	// 小数部分を補完率として使う（0〜1）
	m_t = m_totalDisplaySec - elapsedTime;

	// 分:秒に分解
	const int min = m_totalDisplaySec / 60;
	const int sec = m_totalDisplaySec % 60;

	// 秒（00〜59）
	m_displaySecOnesNum = sec % 10;
	m_displaySecTensNum = (sec / 10) % 10;

	// 分（00〜99想定）
	m_displayMinOnesNum = min % 10;
	m_displayMinTensNum = (min / 10) % 10;
}
