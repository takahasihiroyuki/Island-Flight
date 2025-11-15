#pragma once
class UIScreen
{
public:
	virtual ~UIScreen() = default;

	/// <summary>
	/// •\¦ó‘Ô‚É‚È‚Á‚½‚ÉŒÄ‚Î‚ê‚é
	/// </summary>
	virtual void Open() {};
	/// <summary>
	/// ”ñ•\¦ó‘Ô‚É‚·‚é‚Æ‚«‚ÉŒÄ‚Î‚ê‚ê‚é
	/// </summary>
	virtual void Close() {};

	virtual void Update() = 0;
	virtual void Render(RenderContext& rc) = 0;

	/// <summary>
	/// ‰f‚·‚©‚Ç‚¤‚©
	/// </summary>
	/// <param name="visible"></param>
	virtual void SetVisible(bool visible)
	{
		if (m_visible == visible)return;
		m_visible = visible;

		if (m_visible)Open();
		else Close();
	}

	bool IsVisible() const
	{
		return m_visible;
	}

protected:
	bool m_visible = false;
};

