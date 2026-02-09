#pragma once
#include "UIScreen.h"

class Timer;
class ScoreManager;
class ResultUI :public UIScreen
{
public:
	ResultUI(ScoreManager* scoreManager) :m_scoreManager(scoreManager) {};
	ResultUI() {};
	~ResultUI();
	void OnUpdate()override;
	void Render(RenderContext& rc)override;
	void Init();
	void OnOpen();
	void OnClose();


public:
	void ApplyScore(float score);
private:
	ScoreManager* m_scoreManager = nullptr;
	std::array< SpriteRender, 10> OnesSprite;
	std::array< SpriteRender, 10> TensSprite;
	std::array< SpriteRender, 10> HundredsSprite;
	std::array< SpriteRender, 10> ThousandsSprite;

	SpriteRender m_scoreSprite;
	int m_displayOnesNum = 0;
	int m_displayTensNum = 0;
	int m_displayHundredsNum = 0;
	int m_displayThousandsNum = 0;
	int m_displayNumber = 0;//ï\é¶Ç∑ÇÈêîéö
	std::array<const char*, 10> m_spritePaths;
	Timer* m_timer;
	SoundSource* m_countUpSE;

};

