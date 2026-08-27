#include "Source/stdafx.h"
#include "Source/Gameplay/Score/ComboCounterScreen.h"
#include "Source/Gameplay/Score/ScoreManager.h"

namespace
{
	constexpr float SPRITE_SIZE_X = 90.0f;
	constexpr float SPRITE_SIZE_Y = 100.0f;

	constexpr float COMBO_SPRITE_SIZE_X = 450.0f;
	constexpr float COMBO_SPRITE_SIZE_Y = 100.0f;
}

ComboCounterScreen::ComboCounterScreen()
{
	// 画面全体としての開閉アニメーションはいらない。
	// コンボ残り時間に応じて透明度を変えるため。
	m_animDurationOpen = 0.0f;
	m_animDurationClose = 0.0f;
}

void ComboCounterScreen::Init(ScoreManager* scoreManager)
{
	//スコアマネージャーを注入
	m_scoreManager = scoreManager;


	//スプライトの初期化
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
	}

	//コンボの文字スプライト。
	{
		m_comboSprite.Init(
			"Assets/UI/Combo/combo.DDS",
			COMBO_SPRITE_SIZE_X,
			COMBO_SPRITE_SIZE_Y
		);

		m_comboSprite.SetScale(m_baseScale);
		m_comboSprite.Update();
	}
}

void ComboCounterScreen::OnUpdate()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	if (m_scoreManager == nullptr)
	{
		m_displayComboCount = 0;
		m_alpha = 0.0f;
		return;
	}

	//コンボが続いてない場合
	if (!m_scoreManager->IsComboActive())
	{
		m_displayComboCount = 0;
		m_alpha = 0.0f;
		return;
	}

	//UIを揺らす処理
	{
		int currentComboCount = m_scoreManager->GetComboCount();

		// コンボ数が増えた瞬間に揺れを開始する
		if (currentComboCount > m_prevComboCount)
		{
			StartImpactShake(currentComboCount);
		}

		m_prevComboCount = currentComboCount;
	}

	m_displayComboCount = m_scoreManager->GetComboCount();



	// コンボ残り時間の割合。
	// 1.0 → はっきり表示
	// 0.0 → 完全に消える
	m_alpha = m_scoreManager->GetComboRemainingRate();

	m_alpha = (std::max)(0.0f, (std::min)(m_alpha, 1.0f));

	UpdateImpactShake(deltaTime);
}

void ComboCounterScreen::Render(RenderContext& rc)
{

	//2コンボより少なければ表示しない。
	if (m_displayComboCount < 2)
	{
		return;
	}

	if (m_alpha <= 0.0f)
	{
		return;
	}



	Vector3 renderBasePos = m_basePos + m_impactOffset;

	Vector3 renderScale = Vector3(
		m_baseScale.x * m_impactScaleRate,
		m_baseScale.y * m_impactScaleRate,
		m_baseScale.z
	);

	Vector4 mulColor = Vector4(1.0f, 1.0f, 1.0f, m_alpha);

	RenderComboNumber(rc, m_displayComboCount, renderBasePos, renderScale, mulColor);

	Vector3 comboPos = m_basePos;
	comboPos.x += m_comboSpriteSpacing * m_impactScaleRate;

	m_comboSprite.SetPosition(comboPos);
	m_comboSprite.SetScale(renderScale);
	m_comboSprite.SetMulColor(mulColor);
	m_comboSprite.Update();
	m_comboSprite.Draw(rc);
}

void ComboCounterScreen::RenderComboNumber(
	RenderContext& rc,
	int comboCount,
	const Vector3& basePos,
	const Vector3& scale,
	const Vector4& mulColor
)
{
	if (comboCount < 0)
	{
		comboCount = 0;
	}

	int digitCount = GetDigitCount(comboCount);

	if (digitCount > MAX_DIGIT_COUNT)
	{
		digitCount = MAX_DIGIT_COUNT;
	}

	for (int digitIndex = 0; digitIndex < digitCount; digitIndex++)
	{
		int digit = GetDigit(comboCount, digitIndex);

		Vector3 digitPos = basePos;
		digitPos.x -= m_digitSpacing * digitIndex * m_impactScaleRate;

		m_digitSprites[digitIndex][digit].SetPosition(digitPos);
		m_digitSprites[digitIndex][digit].SetScale(scale);
		m_digitSprites[digitIndex][digit].SetMulColor(mulColor);
		m_digitSprites[digitIndex][digit].Update();
		m_digitSprites[digitIndex][digit].Draw(rc);
	}
}

void ComboCounterScreen::ApplyAlpha(float alpha)
{
	m_comboSprite.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));

	for (int digitIndex = 0; digitIndex < MAX_DIGIT_COUNT; digitIndex++)
	{
		for (int number = 0; number < 10; number++)
		{
			m_digitSprites[digitIndex][number].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
		}
	}
}

int ComboCounterScreen::GetDigitCount(int number) const
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

int ComboCounterScreen::GetDigit(int number, int digitIndex) const
{
	for (int i = 0; i < digitIndex; i++)
	{
		number /= 10;
	}

	return number % 10;
}

void ComboCounterScreen::StartImpactShake(int comboCount)
{
	m_impactElapsedTime = 0.0f;

	// コンボ数に応じて強くする
	float power = m_minImpactPower + comboCount * m_impactPowerPerCombo;

	// 上限をかける
	m_impactPower = Clamp(power, m_minImpactPower, m_maxImpactPower);
}

void ComboCounterScreen::UpdateImpactShake(float deltaTime)
{
	// 揺れの時間が経過したら揺れをリセットする
	if (m_impactElapsedTime >= m_impactDuration)
	{
		m_impactOffset = Vector3::Zero;
		m_impactScaleRate = 1.0f;
		return;
	}

	m_impactElapsedTime += deltaTime;

	float t = m_impactElapsedTime / m_impactDuration;
	t = Clamp(t, 0.0f, 1.0f);

	// 時間とともに揺れを弱くする
	float damping = 1.0f - t;

	float shakeX = sinf(m_impactElapsedTime * m_impactShakeFrequency) * m_impactPower * damping;
	float shakeY = cosf(m_impactElapsedTime * m_impactShakeFrequency * 1.3f) * m_impactPower * 0.5f * damping;

	m_impactOffset = Vector3(shakeX, shakeY, 0.0f);

	// 衝撃波っぽく一瞬だけ大きくして戻す
	m_impactScaleRate = 1.0f + 0.25f * damping;
}

float ComboCounterScreen::Clamp(float value, float minValue, float maxValue) const
{
	if (value < minValue)
	{
		return minValue;
	}

	if (value > maxValue)
	{
		return maxValue;
	}

	return value;
}
