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

void PlacementObject::Initialize(const char* name, const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
	m_modelRender.Init(name);

	m_modelRender.SetPosition(position);
	m_modelRender.SetRotation(rotation);
	m_modelRender.SetScale(scale);
	m_modelRender.Update();

}

void StageMeshObject::Update()
{
}
