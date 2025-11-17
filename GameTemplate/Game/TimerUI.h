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
	void Open();
	void Close();


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
	float m_t;				//補完率
	std::array< SpriteRender, 10> OnesSprite;
	std::array< SpriteRender, 10> TensSprite;
	std::array< SpriteRender, 10> HundredsSprite;
	int m_displayOnesNum = 0;
	int m_displayTensNum = 0;
	int m_displayHundredsNum = 0;
	std::array<const char*, 10> m_spritePaths;
	Timer* m_timer;
};

