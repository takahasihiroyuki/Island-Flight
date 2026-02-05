#pragma once
#include"UIScreen.h"

class Aircraft;
class Timer;
class TitleUI :public UIScreen
{
public:
	TitleUI();
	~TitleUI();
	void OnUpdate()override;
	void Render(RenderContext& rc)override;
	void Init();
	void OnOpen();
	void OnClose();
	void OnOpenAnimUpdate(float t)override;
	void OnCloseAnimUpdate(float t)override;


private:
	SpriteRender m_titleLogo;
	SpriteRender m_titleScreen;
	SpriteRender m_titleStart;
	Aircraft* m_aircraft = nullptr;
	Timer* m_timer;
	Vector3 m_logoPos;
	Vector3 m_StartUIOpenCloseAnimOffset; // 表示、非表示アニメーション用の位置オフセット
	Vector3 m_logoOpenCloseAnimOffset; // 表示、非表示アニメーション用の位置オフセット
	Vector3 m_startPos;

	Vector3 m_startPosBase = Vector3{ 0.0f,-200.0f,0.0f };
	Vector3 m_startScale = Vector3{ 100.0f,100.0f,100.0f };
	Vector3 m_logoShownPos = Vector3{ 0.0f,200.0f,0.0f };			//表示時のポジション
	Vector3 m_logoHiddenPos = Vector3(0.0f, 700.0f, 0.0f);			//非表示時のポジション
	Vector3 m_startShownPos = Vector3{ 0.0f,-200.0f,0.0f };		//表示時のポジション
	Vector3 m_startHiddenPos = Vector3(0.0f, -400.0f, 0.0f);		//非表示時のポジション
};
