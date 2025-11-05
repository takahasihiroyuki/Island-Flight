#pragma once

class PlacementObject : public IGameObject
{
public:
	virtual bool Start() override final;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;


public:
	using TransformTuple = std::tuple<Vector3, Quaternion, Vector3>;

	virtual void Initialize	(
		const char* filePath,
		Vector3 position,
		Quaternion rotation,
		Vector3 scale,
		const char* modelName)final;

	void SetModelName(const char* name)
	{
		m_modelName = _strdup(name);
		float debug = 0;
	}

	Vector3 GetPosition()
	{
		return m_modelRender.GetPosition();
	}

	static size_t GetMaxInstanceCount(const char* name)
	{
		return m_maxInstance[std::string(name)];
	}

	static bool GetIsInstancing(const char* modelName) 
	{
		DecideInstancingUsage(modelName);
		return m_isInstancingTable[modelName];
	}


protected:
	/// <summary>
	/// インスタンシング描画をするかどうかを決める。
	/// 使いたい各オブジェクトのInitが終わったら呼び出す。
	/// </summary>
	/// <param name="isInstancing"></param>
	static void DecideInstancingUsage(const char* modelName);


	virtual void OnStart() {};

	virtual void OnInit() {};

protected:
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;
	ModelRender m_modelRender;
	const char* m_modelName;
	static std::unordered_map<std::string, bool> m_isInstancingTable;
	InstancingManager* m_instancingManager = nullptr;
	static std::unordered_map<std::string, size_t> m_maxInstance;
	size_t m_InstanceNumber = 0;// インスタンスナンバー
	std::string m_modelPath;

};



class StageMeshObject : public PlacementObject
{
private:
	//親クラスをわかりやすくするためのエイリアス
	using SuperClass = PlacementObject;

public:
	virtual void  OnStart() override
	{
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
	virtual void OnStart() override
	{
	}
	virtual void Update() override;
};