#include "stdafx.h"
#include "UIManager.h"

void UIManager::Update()
{
	for (auto& screen : m_screens)
	{
		screen.second->Update();
	}
	
}

void UIManager::Render(RenderContext& rc)
{
	for (auto& screen : m_screens)
	{
		screen.second->Render(rc);
	}
}

void UIManager::ShowScreen(const std::string& name)
{
	auto it = m_screens.find(name);
	if (it != m_screens.end())
	{
		it->second->SetVisible(true);
	}
}

void UIManager::HideScreen(const std::string& name)
{
	auto it = m_screens.find(name);
	if (it != m_screens.end())
	{
		it->second->SetVisible(false);
	}
}
