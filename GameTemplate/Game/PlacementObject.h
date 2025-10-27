#pragma once

class PlacementObject : public IGameObject
{
public:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;


public:
	using TransformTuple = std::tuple<Vector3, Quaternion, Vector3>;
	/// <summary>
	/// 初期化
	/// トランスフォームはタプルで受け取る。
	/// Position,Rotation,Scale
	/// </summary>
	/// <param name="transform"></param>
	void Initialize(const char* name, const TransformTuple& transform);

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
	//親クラスをわかりやすくするためのエイリアス
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