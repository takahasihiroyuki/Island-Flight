#pragma once

class PlacementObject : public IGameObject
{
public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;


public:
	// ”z’uî•ñ‚ğó‚¯æ‚Á‚Äİ’è‚·‚é
	void Initialize(const char* name, const Vector3& position, const Quaternion& rotation, const Vector3& scale);

	Vector3 GetPosition()
	{
		return m_modelRender.GetPosition();
	}

protected:
	ModelRender m_modelRender;
};



class StageMeshObject : public PlacementObject
{
private:
	using SuperClass = PlacementObject;

public:
	virtual bool Start() override
	{
		return true;
	}
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override
	{
		SuperClass::Render(rc);
	}
};

class ItemMeshObject : public PlacementObject
{
private:
	using SuperClass = PlacementObject;

public:
	virtual bool Start() override
	{

	}
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override
	{
		SuperClass::Render(rc);
	}
};