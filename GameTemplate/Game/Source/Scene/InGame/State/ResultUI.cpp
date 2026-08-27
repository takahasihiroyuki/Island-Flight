#include "Source/stdafx.h"
#include "Source/Scene/InGame/State/ResultUI.h"
#include "Source/Gameplay/Score/ScoreManager.h"
#include "Source/UI/UIManager.h"
#include "Source/Gameplay/Time/Timer.h"

namespace
{
	const Vector3 SPRITE_BASE_POS = Vector3(100.0f, 0.0f, 0.0f);
	constexpr float SPACING_X = 75;//数字の隙間の長さ（ｘ）
	const Vector3 ONES_SPLITE_POS = Vector3(SPRITE_BASE_POS.x, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 TENS_SPLITE_POS = Vector3(SPRITE_BASE_POS.x - SPACING_X, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 HUNDREDS_SPLITE_POS = Vector3(SPRITE_BASE_POS.x - SPACING_X * 2, SPRITE_BASE_POS.y, 0.0f);
	const Vector3 THOUSANDS_SPLITE_POS = Vector3(SPRITE_BASE_POS.x - SPACING_X * 3, SPRITE_BASE_POS.y, 0.0f);
	const Vector2 SCORE_SPRITE_SIZE = Vector2(1000.0f, 200.0f * 1.3f);
	const Vector3 SCORE_SPRITE_POSITION = Vector3(0.0f, 300.0f, 0.0f);

	/// <summary>
	/// 線形補完
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	inline float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}
}


ResultUI::~ResultUI()
{
	DeleteGO(m_timer);
	DeleteGO(m_countUpSE);
}

void ResultUI::OnUpdate()
{
	//スコアを時間でカウントアップさせる。
	float t = m_timer->GetElapsedTime();
	float score = m_scoreManager->GetScore();

	//スコアが0のとき効果音停止
	if (score == 0)
	{
		m_countUpSE->Stop();
	}

	if (t > 1.0f)
	{
		t = 1.0f;
		//効果音停止
		if (m_countUpSE->IsPlaying())
		{
			m_countUpSE->Stop();
		}
	}

	//補完計算
	float displayScore = Lerp(0, score, t);

	//スコアを適用
	ApplyScore(displayScore);

}

void ResultUI::Render(RenderContext& rc)
{
	m_scoreSprite.Draw(rc);

	if (m_displayNumber >= 1000)
	{
		ThousandsSprite[m_displayThousandsNum].Draw(rc);
		HundredsSprite[m_displayHundredsNum].Draw(rc);
		TensSprite[m_displayTensNum].Draw(rc);
		OnesSprite[m_displayOnesNum].Draw(rc);
	}
	else if (m_displayNumber >= 100)
	{
		HundredsSprite[m_displayHundredsNum].Draw(rc);
		TensSprite[m_displayTensNum].Draw(rc);
		OnesSprite[m_displayOnesNum].Draw(rc);
	}
	else if (m_displayNumber >= 10)
	{
		TensSprite[m_displayTensNum].Draw(rc);
		OnesSprite[m_displayOnesNum].Draw(rc);
	}
	else
	{
		OnesSprite[m_displayOnesNum].Draw(rc);
	}
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

		TensSprite[i].Init(m_spritePaths[i], 100, 100);
		TensSprite[i].SetPosition(TENS_SPLITE_POS);
		TensSprite[i].Update();

		HundredsSprite[i].Init(m_spritePaths[i], 100, 100);
		HundredsSprite[i].SetPosition(HUNDREDS_SPLITE_POS);
		HundredsSprite[i].Update();

		ThousandsSprite[i].Init(m_spritePaths[i], 100, 100);
		ThousandsSprite[i].SetPosition(THOUSANDS_SPLITE_POS);
		ThousandsSprite[i].Update();

	}

	m_scoreSprite.Init("Assets/UI/Result/SCORE.DDS", SCORE_SPRITE_SIZE.x, SCORE_SPRITE_SIZE.y);
	m_scoreSprite.SetPosition(SCORE_SPRITE_POSITION);
	m_scoreSprite.Update();

	//タイマーを動かす
	m_timer = NewGO<Timer>(0);
	m_timer->SetRunning(true);

	m_countUpSE = NewGO<SoundSource>(0);
	m_countUpSE->Init(static_cast<int>(SoundID::enCountUpSE));
	m_countUpSE->Play(false);
}

void ResultUI::OnOpen()
{
}

void ResultUI::OnClose()
{
	m_countUpSE->Stop();
}

void ResultUI::ApplyScore(float score)
{
	//小数部分切り上げ
	m_displayNumber = static_cast<int>(ceilf(score));


	m_displayOnesNum = m_displayNumber % 10;
	m_displayTensNum = (m_displayNumber / 10) % 10;
	m_displayHundredsNum = (m_displayNumber / 100) % 10;
	m_displayThousandsNum = (m_displayNumber / 1000) % 10;

}
