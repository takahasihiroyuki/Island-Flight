#pragma once
#include"Source/Gameplay/BonusItem/BonusItemType.h"
#include"Source/Gameplay/BonusItem/BonusItemEffectContext.h"

class Timer;
class PlacementObject : public IGameObject
{
public:
	virtual ~PlacementObject();
	virtual bool Start() override final;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;
	/// <summary>
	/// インスタンシングする場合
	/// モデルのポジションを見えない場所に置く
	/// </summary>
	void Deactivate()override final
	{
		IGameObject::Deactivate();

		OnDeactivate();

		//インスタンシングしている場合は見えない場所に移動させる。
		if (m_isInstancingTable[m_modelName])
		{
			m_instancingManager->UpdateInstancingData(
				m_InstanceNumber,
				m_modelName,
				Vector3(10000.0f, -10000.0f, 0),
				m_rotation,
				Vector3::One * 0.0000001f
			);
		}

		//当たり判定オブジェクトを解放する。
		m_physicsStaticObject.Release();
	};

	virtual void OnDeactivate() {}

	/// <summary>
	/// インスタンシングテーブルをクリアする
	/// </summary>
	static void ClearInstancingTables()
	{
		m_maxInstance.clear();
		m_isInstancingTable.clear();
	}

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

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	Vector3 GetPosition() const
	{
		return m_position;
	}

	Quaternion GetRotation() const
	{
		return m_rotation;
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

class CollectibleObject : public PlacementObject
{
private:
	using SuperClass = PlacementObject;

public:

	void Update() override final;

	/// <summary>
	/// オブジェクトを回収する。
	/// 回収された場合はtrueを返す。すでに回収されている場合はfalseを返す。
	/// </summary>
	/// <returns></returns>
	bool Collect()
	{
		if (m_isCollected)
		{
			return false;
		}

		m_isCollected = true;

		OnCollected();
		Deactivate();

		return true;
	}

	void Activate() override
	{
		PlacementObject::Activate();
		m_isCollected = false;
	}


protected:
	virtual void OnUpdate() {};
	virtual void OnCollected() {};
	virtual void OnDeactivate() {};
private:
	bool m_isCollected = false;
};

#include "Source/Gameplay/Stage/PlacementObject.h"
#include "Source/Gameplay/BonusItem/BonusItemMover.h"

/// <summary>
/// ボーナスアイテムオブジェクト
/// BonusItemWaypointSetの情報をもとに移動するアイテムオブジェクト。
/// プレイヤーが回収するとBonusItemEffectContextの内容に従って効果を発動させる。
/// </summary>
class BonusItemWaypointSet;
class BonusItemObject : public CollectibleObject
{
public:

	void OnUpdate() override
	{
		const float deltaTime = g_gameTime->GetFrameDeltaTime();

		m_mover.Update(deltaTime);

		m_position = m_mover.GetPosition();

		m_rotation = m_mover.GetRotation();

	}

	/// <summary>
	/// ボーナスアイテムの移動を初期化する。
	/// </summary>
	/// <param name="waypointSet"></param>
	/// <param name="previousPointIndex"></param>
	/// <param name="currentPointIndex"></param>
	/// <param name="nextPointIndex"></param>
	/// <param name="afterNextPointIndex"></param>
	/// <param name="moveDuration"></param>
	/// <returns></returns>
	bool InitMovement(
		const BonusItemWaypointSet* waypointSet,
		const Vector3& startPosition,
		float moveDuration
	);

	void SetEffectContext(
		const BonusItemEffectContext* context)
	{
		m_effectContext = context;
	}

	/// <summary>
	/// インスタンシング登録に使用するモデル名
	/// </summary>
	/// <returns></returns>
	virtual const char* GetItemModelName() const = 0;

	/// <summary>
	/// インスタンシング登録に使用するモデルパス
	/// </summary>
	virtual const char* GetItemModelPath() const = 0;


	/// <summary>
	/// アイテムの効果を適用する。
	/// </summary>
	virtual void ApplyEffect(const BonusItemEffectContext& context) = 0;


private:

	/// <summary>
	/// 取得されたときの処理
	/// </summary>
	void OnCollected() override final
	{
		if (m_effectContext == nullptr)
		{
			return;
		}

		ApplyEffect(*m_effectContext);
	}

private:
	const BonusItemEffectContext* m_effectContext = nullptr;
	BonusItemMover m_mover;
};