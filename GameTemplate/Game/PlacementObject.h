#pragma once

class PlacementObject : public IGameObject
{
public:
	virtual bool Start() override final;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
	/// <summary>
	/// インスタンシングする場合
	/// モデルのポジションを見えない場所に置く
	/// </summary>
	void Deactivate()override
	{
		IGameObject::Deactivate();
		if (m_isInstancingTable[m_modelName])
		{
			m_instancingManager->UpdateInstancingData(
				m_InstanceNumber,
				m_modelName,
				Vector3(1000000.0f, -1000000.0f, 0),
				m_rotation,
				Vector3::One * 0.0000001f
			);
		}

	};


public:
	using TransformTuple = std::tuple<Vector3, Quaternion, Vector3>;

	virtual void Init(
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

	Matrix GetModelMatrix()
	{
		if (m_isInstancingTable[m_modelName])
		{
			return m_instancingManager->GetModel(m_modelName).GetWorldMatrixArray(m_InstanceNumber);
		}
		return m_modelRender.GetWorldMatrix();
	}

	/// <summary>
	/// 当たり判定オブジェクトを作成
	/// インスタンシングに登録した後で呼び出す。
	/// 
	/// モデルの情報はインスタンシングmanagerが持っているので
	/// インスタンシングに登録した後からしか作れない。
	/// </summary>
	void CreatePhysicsObject()
	{
		if (m_isInstancingTable[m_modelName])
		{
			m_physicsStaticObject.CreateFromModel(
				m_instancingManager->GetModel(m_modelName).GetRenderToGBufferModel(),
				GetModelMatrix());
			return;
		}
		m_physicsStaticObject.CreateFromModel(m_modelRender.GetRenderToGBufferModel(), m_modelRender.GetRenderToGBufferModel().GetWorldMatrix());

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
	Model m_phisycsModel;						//当たり判定を作るためのもでる
	PhysicsStaticObject m_physicsStaticObject;	//当たり判定

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