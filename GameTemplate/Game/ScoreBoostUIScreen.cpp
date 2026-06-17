#include "stdafx.h"
#include "ScoreBoostUIScreen.h"
#include "ScoreManager.h"

namespace
{
	const Vector3 SCORE_BOOST_POSITION =
		Vector3(0.0f, 260.0f, 0.0f);

	const Vector3 SCORE_BOOST_BASE_SCALE =
		Vector3(1.0f, 1.0f, 1.0f);

	constexpr float BLINK_START_SECONDS = 3.0f;

	constexpr float BLINK_INTERVAL = 0.15f;
	constexpr int SCORE_BOOST_SPRITE_WIDTH = 400;
	constexpr int SCORE_BOOST_SPRITE_HEIGHT = 120;
}

ScoreBoostUIScreen::ScoreBoostUIScreen()
{
	// 取得時の飛び出しアニメーション時間
	m_animDurationOpen = 0.35f;

	// 効果終了時の縮小アニメーション時間
	m_animDurationClose = 0.3f;
}

void ScoreBoostUIScreen::Init(const ScoreManager* scoreManager)
{
	m_scoreManager = scoreManager;

	m_scoreBoostSprite.Init(
		"Assets/UI/Numbers/timelimit_zero.DDS",
		SCORE_BOOST_SPRITE_WIDTH,
		SCORE_BOOST_SPRITE_HEIGHT
	);

	m_scoreBoostSprite.SetPosition(
		SCORE_BOOST_POSITION
	);

	m_scoreBoostSprite.SetScale(
		SCORE_BOOST_BASE_SCALE
	);

	m_scoreBoostSprite.Update();

	if (m_scoreManager != nullptr)
	{
		m_observedActivationVersion =
			m_scoreManager
			->GetScoreBoostActivationVersion();
	}
}

void ScoreBoostUIScreen::OnUpdate()
{
	if (m_scoreManager == nullptr)return;

	const std::uint32_t currentVersion = m_scoreManager->GetScoreBoostActivationVersion();

	// 新しくスコアアップアイテムが取得された
	if (currentVersion != m_observedActivationVersion)
	{
		m_observedActivationVersion = currentVersion;

		m_remainingTime = m_scoreManager->GetScoreBoostRemainingTime();

		// 効果中の再取得でも、取得演出を最初から再生する
		Open();
	}

	// 非表示中は、これ以降の表示更新は不要
	if (GetState() == UIState::enHidden)
	{
		return;
	}

	m_remainingTime =
		m_scoreManager
		->GetScoreBoostRemainingTime();

	// スコアアップ効果が終了した
	if (!m_scoreManager->IsScoreBoostActive())
	{
		// 点滅で非表示になったまま閉じないようにする
		m_blinkTimer = 0.0f;
		m_isBlinkVisible = true;

		if (GetState() != UIState::enClosing)
		{
			Close();
		}
	}
	else
	{
		const float deltaTime =
			g_gameTime->GetFrameDeltaTime();

		UpdateBlink(deltaTime);
	}

	ApplyTransform();
}

void ScoreBoostUIScreen::OnOpen()
{
	// 小さい状態から飛び出させる
	m_animationScale = 0.3f;

	// 再取得した場合に点滅状態をリセットする
	m_blinkTimer = 0.0f;
	m_isBlinkVisible = true;

	ApplyTransform();
}

void ScoreBoostUIScreen::OnOpenAnimUpdate(float t)
{
	// 前半：0.3倍から1.2倍まで拡大
	if (t < 0.7f)
	{
		const float firstT = t / 0.7f;

		m_animationScale =
			0.3f + 0.9f * firstT;
	}
	// 後半：1.2倍から1.0倍へ戻す
	else
	{
		const float secondT =
			(t - 0.7f) / 0.3f;

		m_animationScale =
			1.2f - 0.2f * secondT;
	}
}

void ScoreBoostUIScreen::OnCloseAnimUpdate(float t)
{
	// 1.0倍から0.7倍まで縮小する
	m_animationScale =
		1.0f - 0.3f * t;
}

void ScoreBoostUIScreen::UpdateBlink(float deltaTime)
{
	// まだ終了間近ではない場合は常に表示する
	if (m_remainingTime > BLINK_START_SECONDS)
	{
		m_blinkTimer = 0.0f;
		m_isBlinkVisible = true;
		return;
	}

	m_blinkTimer += deltaTime;

	if (m_blinkTimer >= BLINK_INTERVAL)
	{
		m_blinkTimer -= BLINK_INTERVAL;
		m_isBlinkVisible = !m_isBlinkVisible;
	}
}

void ScoreBoostUIScreen::ApplyTransform()
{
	const Vector3 scale =
		SCORE_BOOST_BASE_SCALE
		* m_animationScale;

	m_scoreBoostSprite.SetPosition(
		SCORE_BOOST_POSITION
	);

	m_scoreBoostSprite.SetScale(scale);

	m_scoreBoostSprite.Update();
}

void ScoreBoostUIScreen::Render(RenderContext& rc)
{
	if (!IsRenderable())
	{
		return;
	}

	// 点滅中の非表示タイミング
	if (!m_isBlinkVisible)
	{
		return;
	}

	m_scoreBoostSprite.Draw(rc);
}