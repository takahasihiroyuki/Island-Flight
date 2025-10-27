#include "stdafx.h"
#include "PlacementObject.h"

bool PlacementObject::Start()
{
	return true;
}

void PlacementObject::Update()
{
}

void PlacementObject::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void PlacementObject::Initialize(const char* name, const TransformTuple& transform)
{
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	std::tie(position, rotation, scale) = transform;

	m_modelRender.Init(name);

	m_modelRender.SetPosition(position);
	m_modelRender.SetRotation(rotation);
	m_modelRender.SetScale(scale);
	m_modelRender.Update();

}

void StageMeshObject::Update()
{
}
