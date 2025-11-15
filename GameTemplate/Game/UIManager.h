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

	//スクリーンを登録
	void RegisterScreen(const std::string& name, std::unique_ptr<UIScreen> screen)
	{
		m_screens[name] = std::move(screen);
	};

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
	//シングルトンにしていてInGameObjectに登録できないので
	// InGameObjectがすることをここでする。
	UIManagerObject* m_uiObject;
};