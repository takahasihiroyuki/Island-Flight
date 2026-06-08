#include "stdafx.h"
#include "ScorePopupScreen.h"

namespace
{
	constexpr float SPRITE_SIZE_X = 100.0f;
	constexpr float SPRITE_SIZE_Y = 100.0f;

	constexpr float SCALE_ADD = 0.25f;
	constexpr int MAX_DIGIT_COUNT = 5;
}

ScorePopupScreen::ScorePopupScreen()
{
	// 開閉アニメーションは不要。
	// +100自体が個別にアニメーションするため。
	m_animDurationOpen = 0.0f;
	m_animDurationClose = 0.0f;
}

void ScorePopupScreen::Init()
{
	m_numberSpritePaths[0] = "Assets/UI/Numbers/timelimit_zero.DDS";
	m_numberSpritePaths[1] = "Assets/UI/Numbers/timelimit_one.DDS";
	m_numberSpritePaths[2] = "Assets/UI/Numbers/timelimit_two.DDS";
	m_numberSpritePaths[3] = "Assets/UI/Numbers/timelimit_three.DDS";
	m_numberSpritePaths[4] = "Assets/UI/Numbers/timelimit_four.DDS";
	m_numberSpritePaths[5] = "Assets/UI/Numbers/timelimit_five.DDS";
	m_numberSpritePaths[6] = "Assets/UI/Numbers/timelimit_six.DDS";
	m_numberSpritePaths[7] = "Assets/UI/Numbers/timelimit_seven.DDS";
	m_numberSpritePaths[8] = "Assets/UI/Numbers/timelimit_eight.DDS";
	m_numberSpritePaths[9] = "Assets/UI/Numbers/timelimit_nine.DDS";

	for (int digitIndex = 0; digitIndex < MAX_DIGIT_COUNT; digitIndex++)
	{
		for (int number = 0; number < 10; number++)
		{
			m_digitSprites[digitIndex][number].Init(
				m_numberSpritePaths[number],
				SPRITE_SIZE_X,
				SPRITE_SIZE_Y
			);

			m_digitSprites[digitIndex][number].SetScale(m_baseScale);
			m_digitSprites[digitIndex][number].Update();
		}
	}

	// プラス用スプライト。
	m_plusSprite.Init("Assets/UI/Numbers/plus.DDS", SPRITE_SIZE_X, SPRITE_SIZE_Y);
	m_plusSprite.SetScale(m_baseScale);
	m_plusSprite.Update();
}

void ScorePopupScreen::ShowAddScore(int addScore, const Vector3& screenPosition)
{
	PopupItem item;

	item.addScore = addScore;
	item.position = screenPosition;
	item.elapsedTime = 0.0f;
	item.lifeTime = 0.8f;
	item.moveSpeed = m_moveSpeed;
	item.alpha = 1.0f;
	item.scale = 1.0f;

	m_items.push_back(item);
}

void ScorePopupScreen::OnUpdate()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	for (auto& item : m_items)
	{
		UpdateItem(item, deltaTime);
	}

	m_items.erase(
		std::remove_if(
			m_items.begin(),
			m_items.end(),
			[](const PopupItem& item)
			{
				return item.IsDead();
			}
		),
		m_items.end()
	);
}

void ScorePopupScreen::UpdateItem(PopupItem& item, float deltaTime)
{
	item.elapsedTime += deltaTime;

	float t = item.elapsedTime / item.lifeTime;
	t = (std::min)(t, 1.0f);

	// 上に浮かせる
	item.position.y -= item.moveSpeed * deltaTime;

	// 徐々に透明にする
	item.alpha = 1.0f - t;

	// 少しだけ大きくする
	item.scale = 1.0f + 0.2f * t;
}

void ScorePopupScreen::Render(RenderContext& rc)
{
	for (const auto& item : m_items)
	{
		RenderItem(rc, item);
	}
}

void ScorePopupScreen::RenderItem(RenderContext& rc, const PopupItem& item)
{
	int score = item.addScore;

	if (score < 0)
	{
		score = 0;
	}

	int digitCount = GetDigitCount(score);

	if (digitCount > MAX_DIGIT_COUNT)
	{
		digitCount = MAX_DIGIT_COUNT;
	}

	Vector3 scale = Vector3(
		m_baseScale.x * item.scale,
		m_baseScale.y * item.scale,
		m_baseScale.z
	);

	Vector4 mulColor = Vector4(1.0f, 1.0f, 1.0f, item.alpha);

	// 数字は、1の位を基準位置に置いて、
	// 10の位、100の位を左にずらしていく。
	for (int digitIndex = 0; digitIndex < digitCount; digitIndex++)
	{
		int digit = GetDigit(score, digitIndex);

		Vector3 digitPos = item.position;
		digitPos.x -= m_digitSpacing * digitIndex;

		m_digitSprites[digitIndex][digit].SetPosition(digitPos);
		m_digitSprites[digitIndex][digit].SetScale(scale);
		m_digitSprites[digitIndex][digit].SetMulColor(mulColor);
		m_digitSprites[digitIndex][digit].Update();
		m_digitSprites[digitIndex][digit].Draw(rc);
	}

	// プラス記号は、一番左の数字よりさらに左に置く。
	Vector3 plusPos = item.position;
	plusPos.x -= m_digitSpacing * digitCount;

	m_plusSprite.SetPosition(plusPos);
	m_plusSprite.SetScale(scale);
	m_plusSprite.SetMulColor(mulColor);
	m_plusSprite.Update();
	m_plusSprite.Draw(rc);
}

int ScorePopupScreen::GetDigitCount(int number) const
{
	if (number < 10)
	{
		return 1;
	}

	int count = 0;

	while (number > 0)
	{
		number /= 10;
		count++;
	}

	return count;
}

int ScorePopupScreen::GetDigit(int number, int digitIndex) const
{
	for (int i = 0; i < digitIndex; i++)
	{
		number /= 10;
	}

	return number % 10;
}