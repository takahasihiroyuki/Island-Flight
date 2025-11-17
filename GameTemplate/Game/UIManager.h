#pragma once
#include"UIScreen.h"
#include "UIManagerObject.h"

class UIManager :public Noncopyable
{
public:
	static UIManager& GetInstance()
	{
		static UIManager instance;
		return instance;
	}
	void Update();
	void Render(RenderContext& rc);

	//ƒXƒNƒŠ[ƒ“‚ğ“o˜^
	void RegisterScreen(const std::string& name, std::unique_ptr<UIScreen> screen)
	{
		m_screens[name] = std::move(screen);
	};

	//“o˜^‰ğœ
	void UnregisterScreen(const std::string& name)
	{
		auto it = m_screens.find(name);
		//“ü‚Á‚Ä‚¢‚½‚çÁ‚·B
		if (it != m_screens.end()) {
			m_screens.erase(it);
		}

	}

	void ShowScreen(const std::string& name);
	void HideScreen(const std::string& name);


private:
	UIManager()
	{
		m_uiObject = NewGO<UIManagerObject>(0);
	}
	~UIManager() { DeleteGO(m_uiObject); }

private:
	std::unordered_map<std::string, std::unique_ptr<UIScreen>> m_screens;
	//ƒVƒ“ƒOƒ‹ƒgƒ“‚É‚µ‚Ä‚¢‚ÄInGameObject‚É“o˜^‚Å‚«‚È‚¢‚Ì‚Å
	// InGameObject‚ª‚·‚é‚±‚Æ‚ğ‚±‚±‚Å‚·‚éB
	UIManagerObject* m_uiObject;
};