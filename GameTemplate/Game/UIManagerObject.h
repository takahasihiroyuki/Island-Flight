#pragma once
class UIManagerObject : public IGameObject
{
public:
	UIManagerObject() {};
	~UIManagerObject() {};
	void Render(RenderContext& rc);
};
