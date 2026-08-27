#include "k2EngineLowPreCompile.h"
#include "graphics/Renderer/InstancingManager.h"
namespace nsK2EngineLow {
	bool InstancingManager::Start()
	{
		return true;
	}
	void InstancingManager::RegisterInstancingModels(
		const std::unordered_set<std::string>& modelNameSet,
		const std::unordered_map<std::string, std::string>& paths,
		const std::unordered_map<std::string, bool>& instancingFlags,
		const std::unordered_map<std::string, size_t>& maxInstanceTable
	)
	{

		// ローカルコピーを作り、インスタンシング対象だけ残す
		std::unordered_set<std::string> instancingModelNames = modelNameSet;
		std::unordered_map<std::string, std::string> instancingPaths = paths;


		for (const auto& name : modelNameSet)
		{
			bool useInstancing = false;

			auto itf = instancingFlags.find(name);
			if (itf != instancingFlags.end())
			{
				useInstancing = itf->second;
			}

			if (!useInstancing)
			{
				instancingModelNames.erase(name);
				instancingPaths.erase(name);
			}

		}

		//モデルネームの集合をマージ。
		MergeModelNameSet(instancingModelNames);

		//マックスインスタンスの集合をマージ。
		MergeMaxInstanceTable(maxInstanceTable);

		//インスタンシングするモデルレンダーを作成。
		for (const auto& name : instancingModelNames)
		{
			m_modelRenders[name].Init(
				instancingPaths[name].c_str(),
				nullptr,
				0,
				enModelUpAxisY,
				true,
				m_maxInstanceTable[name],
				false,
				ReflectLayer::enNone
			);
		}
	}
	void InstancingManager::Update()
	{
	}
	void InstancingManager::Render(RenderContext& rc)
	{
		for (const auto& name : m_instancingModelNameSet) {
			m_modelRenders[name].Draw(rc);
		}

		// 描画後、インスタンスデータをクリア
		m_instanceData.clear();
	}
}