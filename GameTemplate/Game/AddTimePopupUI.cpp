#include "stdafx.h"
#include "AddTimePopupUI.h"
#include "UIManager.h"

namespace
{
	const Vector3 POPUP_START_POSITION = Vector3(400.0f, 245.0f, 0.0f);

	const Vector3 POPUP_TARGET_POSITION = Vector3(400.0f, 360.0f, 0.0f);

	const Vector3 POPUP_BASE_SCALE = Vector3(3.0f, 3.0f, 3.0f);

	constexpr float DIGIT_SPACING = 90.0f;

	float EaseOutQuad(float t)
	{
		return 1.0f - (1.0f - t) * (1.0f - t);
	}

	void ApplySpriteTransform(
		SpriteRender& sprite,
		const Vector3& position,
		const Vector3& scale,
		const Vector4& color)
	{
		sprite.SetPosition(position);
		sprite.SetScale(scale);
		sprite.SetMulColor(color);
		sprite.Update();
	}
}

void AddTimePopupUI::Init()
{
	m_numberSpritePaths[0] =
		"Assets/UI/Numbers/timelimit_zero.DDS";
	m_numberSpritePaths[1] =
		"Assets/UI/Numbers/timelimit_one.DDS";
	m_numberSpritePaths[2] =
		"Assets/UI/Numbers/timelimit_two.DDS";
	m_numberSpritePaths[3] =
		"Assets/UI/Numbers/timelimit_three.DDS";
	m_numberSpritePaths[4] =
		"Assets/UI/Numbers/timelimit_four.DDS";
	m_numberSpritePaths[5] =
		"Assets/UI/Numbers/timelimit_five.DDS";
	m_numberSpritePaths[6] =
		"Assets/UI/Numbers/timelimit_six.DDS";
	m_numberSpritePaths[7] =
		"Assets/UI/Numbers/timelimit_seven.DDS";
	m_numberSpritePaths[8] =
		"Assets/UI/Numbers/timelimit_eight.DDS";
	m_numberSpritePaths[9] =
		"Assets/UI/Numbers/timelimit_nine.DDS";

	m_plusSprite.Init(
		"Assets/UI/Numbers/plus.DDS",
		100,
		100
	);

	for (int i = 0; i < 10; i++)
	{
		m_tensSprites[i].Init(
			m_numberSpritePaths[i],
			100,
			100
		);

		m_onesSprites[i].Init(
			m_numberSpritePaths[i],
			100,
			100
		);
	}

	UpdateTransform(0.0f);
}

void AddTimePopupUI::Play(float addedSeconds)
{
	m_addedSeconds = addedSeconds;
	m_elapsedTime = 0.0f;
	m_hasRequestedUnregister = false;

	const int displayNumber = static_cast<int>(addedSeconds);
	m_onesNumber = displayNumber % 10;
	m_tensNumber = (displayNumber / 10) % 10;

	UpdateTransform(0.0f);
	Open();
}

void AddTimePopupUI::OnUpdate()
{
	if (GetState() == UIState::enHidden)
	{
		return;
	}


	m_elapsedTime +=
		g_gameTime->GetFrameDeltaTime();

	const float animationRate =
		(std::min)(
			m_elapsedTime / m_lifetime,
			1.0f
			);

	UpdateTransform(animationRate);

	if (animationRate < 1.0f)
	{
		return;
	}

	if (m_hasRequestedUnregister)
	{
		return;
	}

	m_hasRequestedUnregister = true;

	if (animationRate >= 1.0f)
	{
		Close();
	}
}

void AddTimePopupUI::Render(RenderContext& rc)
{
	m_plusSprite.Draw(rc);

	const int displayNumber =
		static_cast<int>(m_addedSeconds);

	// 10以上なら十の位を描画
	if (displayNumber >= 10)
	{
		m_tensSprites[m_tensNumber].Draw(rc);
	}

	// 一の位を描画
	m_onesSprites[m_onesNumber].Draw(rc);
}

void AddTimePopupUI::UpdateTransform(
	float animationRate)
{
	const float t =
		(std::min)(animationRate, 1.0f);

	const float easedT = EaseOutQuad(t);

	Vector3 position;
	position.Lerp(
		easedT,
		POPUP_START_POSITION,
		POPUP_TARGET_POSITION
	);

	//スケール
	const Vector3 scale =
		POPUP_BASE_SCALE *
		(1.0f - 0.85f * easedT);

	//カラー
	const Vector4 color =
		Vector4(
			0.0f,
			0.9f,
			0.1f,
			1.0f - easedT
		);

	ApplySpriteTransform(
		m_plusSprite,
		position,
		scale,
		color
	);

	const Vector3 tensPosition = position + Vector3(DIGIT_SPACING, 0.0f, 0.0f);

	const Vector3 onesPosition = position + Vector3(DIGIT_SPACING * 2.0f, 0.0f, 0.0f);

	for (int i = 0; i < 10; i++)
	{
		ApplySpriteTransform(
			m_tensSprites[i],
			tensPosition,
			scale,
			color
		);

		ApplySpriteTransform(
			m_onesSprites[i],
			onesPosition,
			scale,
			color
		);
	}
}