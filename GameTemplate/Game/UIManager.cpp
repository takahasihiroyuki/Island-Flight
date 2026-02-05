#include "stdafx.h"
#include "UIManager.h"

void UIManager::Update()
{
	for (auto& screen : m_screens)
	{
		screen.second->Update();
	}

	// “o˜^‰ðœƒŠƒNƒGƒXƒg‚³‚ê‚Ä‚¢‚½‚à‚Ì‚ð“o˜^‰ðœ‚·‚é
	FlushUnregisterRequests();
}

void UIManager::Render(RenderContext& rc)
{
	for (auto& screen : m_screens)
	{
		if (screen.second->IsRenderable())
		{
			screen.second->Render(rc);
		}
	}
}

void UIManager::ShowScreen(const std::string& name)
{
	auto it = m_screens.find(name);
	if (it != m_screens.end())
	{
		it->second->Open();
	}
}

void UIManager::CloseScreen(const std::string& name)
{
	auto it = m_screens.find(name);
	if (it != m_screens.end())
	{
		it->second->Close();
	}
}
