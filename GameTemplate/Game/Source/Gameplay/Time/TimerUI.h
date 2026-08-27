#pragma once
#include"Source/UI/UIScreen.h"
class Timer;
class TimerUI :public UIScreen
{
public:
	TimerUI() {};
	void OnUpdate()override;
	void Render(RenderContext& rc)override;
	void Init(Timer* timer);
	void OnOpen();
	void OnClose();


public:
	/// <summary>
	/// 時間を適用する。
	/// ここのインスタンスを作ったクラスのアップデートで呼ぶ。
	/// </summary>
	/// <param name="elapsedTime">timerの経過時間</param>
	void ApplyTime(float tielapsedTimeme);

	void SetTimer(Timer* timer) { m_timer = timer; }

private:
	int m_totalDisplaySec = 0;//表示する数字
	float m_t;				//補完率
	std::array< SpriteRender, 10> m_minOnesSprite;
	std::array< SpriteRender, 10> m_minTensSprite;
	SpriteRender m_colonSprite;
	std::array< SpriteRender, 10> m_secOnesSprite;
	std::array< SpriteRender, 10> m_secTensSprite;
	SpriteRender m_timerBackSprite;
	int m_displaySecOnesNum = 0;
	int m_displaySecTensNum = 0;
	int m_displayMinOnesNum = 0;
	int m_displayMinTensNum = 0;

	std::array<const char*, 10> m_spritePaths;
	Timer* m_timer;
};

