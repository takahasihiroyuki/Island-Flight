#pragma once
#include "UIScreen.h"
class ScoreManager;
class ResultUI :public UIScreen
{
public:
	ResultUI(ScoreManager* scoreManager) :m_scoreManager(scoreManager) {};
	ResultUI() {};
	~ResultUI();
	void Update()override;
	void Render(RenderContext& rc)override;
	void Init();
	void Open() {};
	void Close();


public:
	void ApplyScore(float score);
private:
	ScoreManager* m_scoreManager = nullptr;
	std::array< SpriteRender, 10> OnesSprite;
	std::array< SpriteRender, 10> TensSprite;
	std::array< SpriteRender, 10> HundredsSprite;
	int m_displayOnesNum = 0;
	int m_displayTensNum = 0;
	int m_displayHundredsNum = 0;
	int m_displayNumber = 0;//•\¦‚·‚é”š
	std::array<const char*, 10> m_spritePaths;

};

