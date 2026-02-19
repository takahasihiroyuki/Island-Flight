#pragma once
#include "UIScreen.h"

class PlayerAssistUI :public UIScreen
{
public:
	PlayerAssistUI();
	~PlayerAssistUI() {};
	void OnUpdate()override;
	void Render(RenderContext& rc)override;
	void Init();
	void OnOpen();
	void OnClose();
	void OnOpenAnimUpdate(float t)override;
	void OnCloseAnimUpdate(float t)override;

private:
	SpriteRender m_ui;

	Vector3 m_uiShownPos = Vector3{ -630.0f,-350.0f,0.0f };		//表示時のポジション
	Vector3 m_uiHiddenPos = Vector3(-1200.0f, -350.0f, 0.0f);		//非表示時のポジション

	Vector3 m_uiOpenCloseAnimOffset; // 表示、非表示アニメーション用の位置オフセット



};

