#pragma once
#include <Windows.h>
enum class UIState
{
	enHidden,
	enOpening,
	enShown,
	enClosing,
};
/// <summary>
/// UIの画面の基底クラス
/// UIは3DモデルでもOK
/// </summary>
class UIScreen
{
public:
	virtual ~UIScreen() = default;

	/// <summary>
	/// 表示状態になった時に呼ばれる
	/// </summary>
	virtual void Open()final {
		m_state = UIState::enOpening;
		m_animTimer = 0.0f;

		OnOpen();

		// 表示時のアニメーションしないならShownにする
		if (!HasOpenAnim()) {
			m_state = UIState::enShown;
		}
	};

	/// <summary>
	/// 派生クラスで表示状態になったときの処理を実装する
	/// </summary>
	virtual void OnOpen() {};

	/// <summary>
	/// 非表示状態にするときに呼ばれれる
	/// </summary>
	virtual void Close()final {
		m_state = UIState::enClosing;
		m_animTimer = 0.0f;

		OnClose();

		// 非表示のアニメーションしないなら即Hiddenへ
		if (!HasCloseAnim()) {
			m_state = UIState::enHidden;;
		}

	};

	/// <summary>
	/// 派生クラスで非表示状態になるときの処理を実装する
	/// </summary>
	virtual void OnClose() {};

	virtual void Update()final
	{
		float deltaTime = g_gameTime->GetFrameDeltaTime();

		switch (m_state)
		{
		case UIState::enOpening:
			OpenAnimUpdate(deltaTime);
			break;
		case UIState::enClosing:
			CloseAnimUpdate(deltaTime);
			break;
		default:
			break;
		}
		//共通の更新処理
		//ハイドの時も呼ばれる、分岐は派生側でする。
		OnUpdate();
	}
	/// <summary>
	/// 派生クラスで共通の更新処理を実装する
	/// ハイドの時も呼ばれる、分岐は派生側でする。
	/// </summary>
	virtual void OnUpdate() = 0;
	virtual void Render(RenderContext& rc) = 0;

	/// <summary>
	/// 表示時のアニメーションの更新
	/// 表示アニメーションしない場合は、何も書かない。
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void OpenAnimUpdate(float deltaTime) {
		m_animTimer += deltaTime;
		if (m_animTimer >= m_animDurationOpen) {
			//アニメーション終了
			m_animTimer = 0.0f;
			m_state = UIState::enShown;
		}
		else {
			float t = m_animTimer / m_animDurationOpen;
			t = (std::min)(t, 1.0f);
			OnOpenAnimUpdate(t);//アニメーション中
		}
	};
	virtual void OnOpenAnimUpdate(float t) {};
	/// <summary>
	/// 非表示時のアニメーションの更新
	/// 非表示アニメーションしない場合は、何も書かない。
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void CloseAnimUpdate(float deltaTime) {
		m_animTimer += deltaTime;
		if (m_animTimer >= m_animDurationClose) {
			//アニメーション終了
			m_animTimer = 0.0f;
			m_state = UIState::enHidden;
		}
		else {
			float t = m_animTimer / m_animDurationClose;
			t = (std::min)(t, 1.0f);
			OnCloseAnimUpdate(t);//アニメーション中
		}
	};

	/// <summary>
	/// 非表示時のアニメーション更新処理を派生クラスで実装する
	/// </summary>
	/// <param name="t">非表示アニメーションの進行度（0.0f ～ 1.0f）</param>
	virtual void OnCloseAnimUpdate(float t) {};

	UIState GetState() const
	{
		return m_state;
	}

	/// <summary>
	/// 描画するかどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsRenderable()const
	{
		return m_state != UIState::enHidden;
	}

	void SetName(const char* name)
	{
		m_name = name;
	}

	const char* GetName() const
	{
		return m_name;
	}

	/// <summary>
	/// UIが閉じるときのアニメーションがあるかどうか
	/// </summary>
	/// <returns></returns>
	const bool HasCloseAnim() const {
		return m_animDurationClose > 0.0f;
	}

	/// <summary>
	/// UIが開くときのアニメーションがあるかどうか
	/// </summary>
	/// <returns></returns>
	const bool HasOpenAnim() const {
		return m_animDurationOpen > 0.0f;
	}

protected:
	const char* m_name = nullptr;	//スクリーンの名前
	UIState m_state;
	float m_animTimer = 0;			//アニメーション用タイマー
	float m_animDurationOpen = 0;	//UI表示時のアニメーションの時間（0ならアニメーションしない）
	float m_animDurationClose = 0;	//UI非表示時のアニメーションの時間（0ならアニメーションしない）
};

