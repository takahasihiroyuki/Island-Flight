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

	/// <summary>
	/// スクリーンを登録する。
	/// </summary>
	/// <param name="name"></param>
	/// <param name="screen"></param>
	void RegisterScreen(const std::string& name, std::unique_ptr<UIScreen> screen)
	{
		m_screens[name] = std::move(screen);
		m_screens[name]->SetName(name);
	}

	/// <summary>
	/// 登録解除をリクエストする。
	/// スクリーンのアップデート中などに解除されないように、
	/// リクエストをためておいて、後でまとめて解除する。
	/// 各スクリーンクラスのクローズでは呼ばない。
	/// スクリーンのオブジェクトを持っているクラスが呼ぶ。
	/// </summary>
	/// <param name=""></param>
	void RequestUnregisterScreen(const std::string& name)
	{
		m_pendingUnregister.push_back(name);
	}

	void ShowScreen(const std::string& name);

	/// <summary>
	/// スクリーンを閉じる。
	/// FlushUnregisterRequests()を読んでもクローズはされる。
	/// でも登録解除せず、一時的に消したい時などはこっちを呼ぶ。
	/// </summary>
	/// <param name="name"></param>
	void CloseScreen(const std::string& name);


private:
	UIManager()
	{
		m_uiObject = NewGO<UIManagerObject>(0);
	}
	~UIManager() { DeleteGO(m_uiObject); }

	/// <summary>
	///	登録解除リクエストされていたものを登録解除する
	/// リクエストされていても、まだ非表示時のアニメーションが実行されていたら、
	/// まだ登録解除したくないので、ハイド状態になるまで待つ。
	/// クローズ関数がまだ呼ばれていなければクローズする。
	/// </summary>
	/// <param name="name"></param>
	void FlushUnregisterRequests()
	{
		// 登録解除リクエストされたリストを見る
		for (auto it = m_pendingUnregister.begin();
			it != m_pendingUnregister.end(); )
		{
			const std::string& name = *it;
			// スクリーンのイテレーターを取得
			auto screenIt = m_screens.find(name);

			// そのスクリーンが存在するか確認
			if (screenIt == m_screens.end())
			{
				// スクリーンが存在しないなら
				// リクエストを削除して次のイテレーターを指す
				it = m_pendingUnregister.erase(it);
				continue;
			}

			// スクリーンの状態を取得
			UIState screenState = screenIt->second->GetState();

			// 非表示になってたらスクリーンを削除
			if (screenState == UIState::enHidden)
			{
				// スクリーンを削除
				m_screens.erase(screenIt);
				// リクエストも削除して次のイテレーターを指す
				it = m_pendingUnregister.erase(it);
				continue;
			}

			// もしまだ表示中や表示アニメーション中なら閉じるように指示を出す
			if (screenState == UIState::enShown ||
				screenState == UIState::enOpening)
			{
				CloseScreen(name);
			}

			//ここまで来たら次のイテレーターへ
			++it;
		}

	}

private:
	std::unordered_map<std::string, std::unique_ptr<UIScreen>> m_screens;
	//シングルトンにしていてInGameObjectに登録できないので
	// InGameObjectがすることをここでする。
	UIManagerObject* m_uiObject;
	std::vector<std::string> m_pendingUnregister;// 登録解除リクエストされたスクリーン名のリスト
};