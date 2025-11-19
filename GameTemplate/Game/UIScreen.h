#pragma once
class UIScreen
{
public:
	virtual ~UIScreen() = default;

	/// <summary>
	/// 表示状態になった時に呼ばれる
	/// </summary>
	virtual void Open() {};
	/// <summary>
	/// 非表示状態にするときに呼ばれれる
	/// </summary>
	virtual void Close() {};

	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

	/// <summary>
	/// 映すかどうか
	/// </summary>
	/// <param name="displayed"></param>
	virtual void SetDisplayed(bool displayed)
	{
		if (m_displayed == displayed)return;
		m_displayed = displayed;

		if (m_displayed)Open();
		else Close();
	}

	/// <summary>
	/// 表示するかどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsDisplayed()
	{
		return m_displayed;
	}

protected:
	bool m_displayed = false;//表示するかどうか
};

