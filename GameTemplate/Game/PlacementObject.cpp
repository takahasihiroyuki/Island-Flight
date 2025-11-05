#include "stdafx.h"
#include "PlacementObject.h"

std::unordered_map<std::string, size_t> PlacementObject::m_maxInstance;
std::unordered_map<std::string, bool>   PlacementObject::m_isInstancingTable{};


bool PlacementObject::Start()
{

	//インスタンシングするか決める。
	DecideInstancingUsage(m_modelName);
	if (m_isInstancingTable[m_modelName])
	{
		m_instancingManager = FindGO<InstancingManager>("instancingManager");
		if (m_instancingManager)
		{
			m_instancingManager->UpdateInstancingData(
				m_InstanceNumber,
				m_modelName,
				m_position,
				m_rotation,
				m_scale
			);
		}
	}
	else {
		m_modelRender.Init(m_modelPath.c_str());
		m_modelRender.SetPosition(m_position);
		m_modelRender.SetRotation(m_rotation);
		m_modelRender.SetScale(m_scale);
		m_modelRender.Update();
	}

	OnStart();
	return true;
}

void PlacementObject::Update()
{
}

void PlacementObject::Render(RenderContext& rc)
{
	if (!m_isInstancingTable[m_modelName]) {
		m_modelRender.Draw(rc);
	}
}

/// <summary>
/// インスタンシング描画するかどうか決める。
/// マックスインスタンスが1以下ならインスタンシング描画しない
/// </summary>
void PlacementObject::DecideInstancingUsage(const char* modelName)
{
	//マックスインスタンスが1より多ければインスタンシング描画を有効にする。
	if (m_maxInstance[std::string(modelName)] > 1)
	{
		m_isInstancingTable[modelName] = true;
	}
	else m_isInstancingTable[modelName] = false;
}

void PlacementObject::Initialize(
	const char* filePath,
	Vector3 position,
	Quaternion rotation, 
	Vector3 scale,
	const char* modelName)
{
	m_position=position;
	m_rotation=rotation;
	m_scale=scale;

	m_InstanceNumber = m_maxInstance[std::string(modelName)];

	//インスタンシング描画する場合ここではモデルレンダーは作らない。
	//一種類ひとつのモデルレンダーを作りたいから。
	m_maxInstance[std::string(modelName)]++;
	SetModelName(modelName);

	//インスタンシング描画するならこのモデルレンダーは使わないが、
	//この段階ではまだ、インスタンシング描画するか判断できないので
	//初期化しておく
	m_modelPath = filePath;

	OnInit();
}

void StageMeshObject::Update()
{
}

void ItemMeshObject::Update()
{
}

