#pragma once
#include"UIScreen.h"

class Timer;
class TitleUI :public UIScreen
{
public:
	TitleUI() {};
	~TitleUI();
	void Update()override;
	void Render(RenderContext& rc)override;
	void Init();
	void Open();
	void Close();


private:
	SpriteRender m_titleLogo;
	SpriteRender m_titleScreen;
	SpriteRender m_titleStart;
	Timer* m_timer;
	Vector3 m_logoPos;
	Vector3 m_logoPosBase = Vector3{ 0.0f,200.0f,0.0f };

	Vector3 m_startPos;
	Vector3 m_startPosBase = Vector3{ 0.0f,-200.0f,0.0f };
	Vector3 m_startScale = Vector3{ 100.0f,100.0f,100.0f };

};

