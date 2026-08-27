#pragma once
#include "Source/UI/UIScreen.h"

class AddTimePopupUI final : public UIScreen
{
public:
	void Init();

	void Play(float addedSeconds);
	void OnUpdate() override;
	void Render(RenderContext& rc) override;

private:
	void UpdateTransform(float animationRate);

private:
	float m_addedSeconds = 0.0f;

	float m_elapsedTime = 0.0f;
	float m_lifetime = 1.2f;

	bool m_hasRequestedUnregister = false;

	int m_onesNumber = 0;
	int m_tensNumber = 0;

	SpriteRender m_plusSprite;

	std::array<SpriteRender, 10>
		m_onesSprites;

	std::array<SpriteRender, 10>
		m_tensSprites;

	std::array<const char*, 10>
		m_numberSpritePaths;
};