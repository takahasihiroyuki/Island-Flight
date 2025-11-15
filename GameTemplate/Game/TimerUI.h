#pragma once
#include"UIScreen.h"
class Timer;
class TimerUI :public UIScreen
{
public:
	TimerUI() {};
	void Update()override;
	void Render(RenderContext& rc)override;
	void Init(Timer* timer);


public:
	/// <summary>
	/// 時間を適用する。
	/// ここのインスタンスを作ったクラスのアップデートで呼ぶ。
	/// </summary>
	/// <param name="elapsedTime">timerの経過時間</param>
	void ApplyTime(float tielapsedTimeme);

	void SetTimer(Timer* timer) { m_timer = timer; }

private:
	int m_displayNumber = 0;//表示する数字
	Vector3 m_size;
	float m_t;				//補完率
	float m_colorAlpha;
	SpriteRender OnesSprite;
	SpriteRender TensSprite;
	SpriteRender HundredsSprite;
	int m_displayOnesNum = 0;
	int m_displayTensNum = 0;
	int m_displayHundredsNum = 0;
	int m_prevDisplayOnesNum = 0;
	int m_prevDisplayTensNum = 0;
	int m_prevDisplayHundredsNum = 0;
	std::array<const char*, 10> m_spritePaths;
	Timer* m_timer;

};

