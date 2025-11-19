#include "stdafx.h"
#include "TitleUI.h"
#include"UIManager.h"
#include"Timer.h"

namespace {
	const Vector3 ScaleOffsetLogo = Vector3(10.0f, 5.0f, 0.0f);
	const Vector3 ScaleOffsetStart = Vector3(4.0f, 2.0f, 0.0f);
	const Vector3 ScaleOffsetScreen = Vector3(20.0f, 15.0f, 0.0f);

}


TitleUI::~TitleUI()
{
	
}

void TitleUI::Update()
{
	float t = m_timer->GetElapsedTime();

	m_logoPos = m_logoPosBase;
	m_logoPos.y = m_logoPosBase.y + 150 * abs(cos(t * 7)) * std::exp(-t * 0.9);

	m_startScale = ScaleOffsetStart;
	m_startScale += Vector3::One * 0.5 * abs(std::sin(t));


	m_titleLogo.SetPosition(m_logoPos);
	m_titleStart.SetPosition(m_startPosBase);
	m_titleLogo.SetScale(ScaleOffsetLogo);
	m_titleStart.SetScale(m_startScale);
	m_titleScreen.SetScale(ScaleOffsetScreen);
	m_titleStart.Update();
	m_titleLogo.Update();
	m_titleScreen.Update();
}

void TitleUI::Render(RenderContext& rc)
{
	m_titleScreen.Draw(rc);
	m_titleLogo.Draw(rc);
	m_titleStart.Draw(rc);

}

void TitleUI::Init()
{
	m_titleLogo.Init("Assets/UI/Title/TitleRogo.DDS", 100.0f, 100.0f);
	m_titleLogo.Update();
	m_titleScreen.Init("Assets/UI/Title/TitleScren.DDS", 100.0f, 100.0f);
	m_titleScreen.Update();

	m_titleStart.Init("Assets/UI/Title/TitleStart.DDS", 100.0f, 100.0f);
	m_titleStart.Update();

}

void TitleUI::Open()
{
	m_timer = NewGO<Timer>(0);
	//タイマーを動かす
	m_timer->SetRunning(true);
}

void TitleUI::Close()
{
	UIManager::GetInstance().UnregisterScreen("timerUI");
	DeleteGO(m_timer);
}
