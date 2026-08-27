#pragma once
#include <unordered_set>

namespace nsK2EngineLow {
	class InstancingManager :public IGameObject
	{
	public:
		InstancingManager() {};
		~InstancingManager() {};

		bool Start() override;

		/// <summary>
		/// モデル一覧からインスタンシング対象のモデルのみを選別し,
		/// 対応する ModelRender を生成し登録する。
		/// </summary>
		/// <param name="modelNameSet">すべてのモデル名の集合</param>
		/// <param name="paths">モデル名をキーとしたモデルファイルのパス</param>
		/// <param name="instancingFlags">モデルごとのインスタンシング使用フラグ</param>
		void RegisterInstancingModels(
			const std::unordered_set<std::string>& modelNameSet,
			const std::unordered_map<std::string, std::string>& paths,
			const std::unordered_map<std::string, bool>& instancingFlags,
			const std::unordered_map<std::string, size_t>& maxInstanceTable
		);
		void Update() override;
		void Render(RenderContext& rc) override;

		void MergeModelNameSet(const std::unordered_set<std::string>& modelNameSet) {
			//引数の配列とメンバの配列をマージする。
			//かぶったら上書き。
			for (std::unordered_set<std::string>::const_iterator it = modelNameSet.begin();
				it != modelNameSet.end(); ++it)
			{
				m_instancingModelNameSet.insert(*it);
			}
		}

		void MergeMaxInstanceTable(const std::unordered_map<std::string, size_t>& maxInstanceTable)
		{
			//引数のマックスインスタンス配列とメンバの配列をマージする。
			//かぶったら上書き。
			for (std::unordered_map<std::string, size_t>::const_iterator it = maxInstanceTable.begin();
				it != maxInstanceTable.end(); ++it)
			{
				m_maxInstanceTable[it->first] = it->second;
			}
		}

		const ModelRender& GetModel(std::string modelName)const
		{
			return m_modelRenders.at(modelName);
		}
		ModelRender& GetModel(std::string modelName)
		{
			return m_modelRenders.at(modelName);
		}

		void UpdateInstancingData(
			int instanceNo,
			std::string instanceName,
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		) {
			m_modelRenders[instanceName].UpdateInstancingData(
				instanceNo,
				instanceName.c_str(),
				pos,
				rot,
				scale
			);
		}

		void SetMaxInstanceCount(const char* objectName, int maxInstance) {
			m_maxInstanceTable[std::string(objectName)]=maxInstance;
		}


	private:
		std::unordered_map<std::string,ModelRender> m_modelRenders;
		std::unordered_map<std::string, std::vector<Matrix>> m_instanceData;
		std::unordered_map<std::string, size_t> m_maxInstanceTable;
		size_t m_instanceNum;
		std::unordered_set<std::string> m_instancingModelNameSet;
	};
}

