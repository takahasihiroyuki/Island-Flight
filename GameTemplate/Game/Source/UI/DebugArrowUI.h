#pragma once
#include"Source/UI/UIScreen.h"

enum collarType
{
	enYerrow,
	enRed,
	enBlue,
};

class DebugArrowUI :public UIScreen
{
public:
	DebugArrowUI()
	{}
	~DebugArrowUI() {};
	void OnUpdate()override;
	void Render(RenderContext& rc)override;
	void Init(collarType collar, float length, float scale = 1.0f);
	void OnOpen();
	void OnClose();
	void UpdateTargetVec(const Vector3& moment)
	{
		m_targetVec = moment;
	}
	void UpdatePosition(const Vector3& position)
	{
		m_position = position;
	}

private:
	Vector3 m_targetVec;
	Vector3 m_position;
	float m_baseLength = 1;
	float m_baseScale = 1;
	ModelRender m_arrow;


};

