#pragma once
#include "Source/UI/UIScreen.h"

class ScorePopupScreen : public UIScreen
{
public:
	ScorePopupScreen();
	~ScorePopupScreen() override = default;

	void Init();

	/// <summary>
	/// ‰ÁZƒXƒRƒA•\¦‚ğ’Ç‰Á‚·‚éB
	/// </summary>
	void ShowAddScore(int addScore, const Vector3& screenPosition);

private:
	struct PopupItem
	{
		int addScore = 0;

		Vector3 position = Vector3::Zero;

		float elapsedTime = 0.0f;
		float lifeTime = 0.8f;

		float moveSpeed = 80.0f;
		float alpha = 1.0f;
		float scale = 1.0f;

		bool IsDead() const
		{
			return elapsedTime >= lifeTime;
		}
	};

private:
	void OnUpdate() override;
	void Render(RenderContext& rc) override;

	void UpdateItem(PopupItem& item, float deltaTime);
	void RenderItem(RenderContext& rc, const PopupItem& item);

	/// <summary>
	/// “n‚³‚ê‚½”š‚ÌŒ…”‚ğ•Ô‚·
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	int GetDigitCount(int number) const;

	/// <summary>
	/// “n‚³‚ê‚½”‚Ì“n‚³‚ê‚½Œ…‚ğ•Ô‚·
	/// </summary>
	/// <param name="number"></param>
	/// <param name="digitIndex"></param>
	/// <returns></returns>
	int GetDigit(int number, int digitIndex) const;
private:
	static constexpr int MAX_DIGIT_COUNT = 5;

	std::vector<PopupItem> m_items;
	SpriteRender m_plusSprite;
	SpriteRender m_digitSprites[MAX_DIGIT_COUNT][10];
	const char* m_numberSpritePaths[10];

	float m_moveSpeed = 90.0f;
	float m_digitSpacing = 45.0f;
	Vector3 m_baseScale = Vector3(0.6f, 0.6f, 0.6f);
};

