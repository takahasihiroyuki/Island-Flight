#pragma once

#include "Source/UI/UIScreen.h"
#include <cstdint>

class ScoreManager;

class ScoreBoostUIScreen final : public UIScreen
{
public:
	ScoreBoostUIScreen();
	~ScoreBoostUIScreen() override = default;

	static constexpr const char* GetScreenName()
	{
		return "ScoreBoostUIScreen";
	}

	void Init(const ScoreManager* scoreManager);

	void OnUpdate() override;
	void Render(RenderContext& rc) override;

protected:
	void OnOpen() override;


	void OnOpenAnimUpdate(float t) override;

	void OnCloseAnimUpdate(float t) override;

private:
	/// <summary>
	/// 効果終了が近い場合の点滅処理。
	/// </summary>
	void UpdateBlink(float deltaTime);

	void ApplyTransform();

private:
	const ScoreManager* m_scoreManager = nullptr;

	SpriteRender m_scoreBoostSprite;
	std::uint32_t m_observedActivationVersion = 0;
	float m_remainingTime = 0.0f;
	float m_animationScale = 1.0f;

	// 点滅
	float m_blinkTimer = 0.0f;
	bool m_isBlinkVisible = true;
};