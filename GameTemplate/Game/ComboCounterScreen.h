#pragma once
#include "UIScreen.h"

/// <summary>
/// 現在のコンボ数を表示するUI。
/// コンボ残り時間に合わせて徐々に消える。
/// </summary>
class ScoreManager;
class ComboCounterScreen : public UIScreen
{
public:
	ComboCounterScreen();
	~ComboCounterScreen() override = default;

	void Init(ScoreManager* scoreManager);

private:
	void OnUpdate() override;
	void Render(RenderContext& rc) override;

	void RenderComboNumber(
		RenderContext& rc,
		int comboCount,
		const Vector3& basePos,
		const Vector3& scale,
		const Vector4& mulColor
	);

	void ApplyAlpha(float alpha);

	int GetDigitCount(int number) const;
	int GetDigit(int number, int digitIndex) const;

	///////UIを揺らすための関数//////////

	void StartImpactShake(int comboCount);
	void UpdateImpactShake(float deltaTime);
	float Clamp(float value, float minValue, float maxValue) const;

private:
	static constexpr int MAX_DIGIT_COUNT = 3;

	ScoreManager* m_scoreManager = nullptr;

	SpriteRender m_comboSprite;
	SpriteRender m_digitSprites[MAX_DIGIT_COUNT][10];

	const char* m_numberSpritePaths[10];

	int m_displayComboCount = 0;
	float m_alpha = 0.0f;

	Vector3 m_basePos = Vector3(0.0f, 180.0f, 0.0f);
	Vector3 m_baseScale = Vector3(0.75f, 0.75f, 0.75f);

	float m_digitSpacing = 55.0f;
	float m_comboSpriteSpacing = 200.0f;


	////////UIを揺らすための変数//////////
	int m_prevComboCount = 0;

	float m_impactElapsedTime = 0.0f;
	float m_impactDuration = 0.25f;		//何秒揺れるか

	float m_impactPower = 0.0f;
	float m_minImpactPower = 4.0f;
	float m_maxImpactPower = 22.0f;
	float m_impactPowerPerCombo = 2.0f;

	float m_impactShakeFrequency = 45.0f;

	Vector3 m_impactOffset = Vector3::Zero;
	float m_impactScaleRate = 1.0f;
};