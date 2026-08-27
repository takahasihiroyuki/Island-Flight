#include "Source/stdafx.h"
#include "Source/Gameplay/Stage/Stage.h"
#include "SceneLoader/SceneLoader.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include"Source/Gameplay/Coin/CoinManager.h"
#include "Source/Gameplay/Coin/Coin.h"
#include "Source/Gameplay/Stage/PlacementObject.h"
#include "Source/Gameplay/BonusItem/BonusItemManager.h"


namespace
{
	//scale:pos=1:39がベスト
	constexpr float OBJECT_SCALE_OFFSET = 10.0f * 0.9;
	constexpr float OBJECT_POS_OFFSET = 390.0f;

	//各オブジェクトごとに objectNames を線形探索すると全体で O(N²) になるため
	//高速に存在判定できる unordered_set にしておく。

	// 名前の集合を作成。

	const std::unordered_set<std::string> stageObjectNameSet =
	{
		"BananaTree_1",
		"BananaTree_2",
		"BananaTree_3",
		"Barrel_1",
		"Block_Bridge",
		"Block_L_1",
		"Block_L_2",
		"Block_L_3",
		"Block_S_1",
		"Block_S_2",
		"Block_S_3",
		"Block_T",
		"Bones_Head",
		"Bones_Skull",
		"Bones_Torso",
		"Bridge",
		"Bucket_1",
		"Bucket_2",
		"Coin",
		"Canon",
		"CanonBall",
		"CanonBalls",
		"Cart",
		"Cart_Broken",
		"Column_Egypt_2",
		"Column_Egypt_3",
		"Column_V1_1",
		"Column_V1_2",
		"Column_V1_3",
		"Column_V1_4",
		"Column_V2_1",
		"Column_V2_2",
		"Column_V2_3",
		"Crane_1",
		"Crane_2",
		"Crane_3",
		"Crate_1",
		"Crate_2",
		"Elephant_Statue",
		"Fence_1",
		"Fence_2",
		"Fence_3",
		"Fence_4",
		"Footbridge",
		"Island_1",
		"Island_2",
		"Island_3",
		"Ladder",
		"Ladder_Broken",
		"Palm_1",
		"Palm_2",
		"Palm_3",
		"Palm_4",
		"Palm_5",
		"Palm_6",
		"Palm_7",
		"Palm_8",
		"Palm_9",
		"Palm_10",
		"Plant_1",
		"Plant_2",
		"Plant_3",
		"Plant_4",
		"Plant_5",
		"Plant_6",
		"Plant_7",
		"Plant_8",
		"Plant_9",
		"Plant_10",
		"Pyramid_L",
		"Pyramid_M",
		"Pyramid_S",
		"Rock_1",
		"Rock_2",
		"Rock_3",
		"Rock_4",
		"Rock_5",
		"Rock_6",
		"Rock_7",
		"Rock_8",
		"Rock_9",
		"Rock_10",
		//"Rock_Formation_1",
		"Shipwreck",
		"Spear",
		"StartPosition",
		"Stall_1",
		"Stall_2",
		"Stall_3",
		"Stone_1",
		"Stone_2",
		"Stone_3",
		"Tent_1",
		"Tent_2",
		"Tent_3",
		"Tile_1",
		"Tile_2",
		"Tile_3",
		"Tile_4",
		"Tile_5",
		"Tile_6",
		"Tile_7",
		"Tile_8",
		"Treasure_Chest",
		"Vase_L_1",
		"Vase_L_2",
		"Vase_L_3",
		"Vase_M_1",
		"Vase_M_2",
		"Vase_M_3",
		"Vase_S_1",
		"Vase_S_2",
		"Vase_S_3",
		"WaterVase_1",
		"WaterVase_2",
		"WaterVase_3",
		"WoodenLog",
		"WoodenLog_2",
		"Wooden_Box",
		"stuckRecoveryPos",
		"AddTimeItem",
		"ScoreBoostItem",
		"BonusItemWaypoint"
	};

	/// <summary>
	/// ターゲットとJSON文字列が等しいか調べる。
	/// </summary>
	/// <param name="json"></param>
	/// <param name="target"></param>
	/// <returns></returns>
	bool IsJsonStringEqual(const char* json, const char* target)
	{
		auto len = strlen(target);
		auto namelen = strlen(json);
		if (len > namelen) {
			//名前が長い。
			return false;
		}
		if (strncmp(json, target, len) == 0) {
			return true;
		}
		return false;
	}

	/// <summary>
	/// 名前の配列の要素とJSON文字列が等しいか調べる。
	/// </summary>
	/// <param name="json"></param>
	/// <returns></returns>
	bool IsJsonStringEqualToAny(const char* json)
	{
		for (const auto& target : stageObjectNameSet) {
			if (IsJsonStringEqual(json, target.c_str())) {
				return true;
			}
		}
		return false;
	}
};


Stage::~Stage()
{
	DeleteGO(m_instancingManager);
	for (auto* obj : m_stageObjects) {
		DeleteGO(obj);
	}
	m_stageObjects.clear();
}

void Stage::Init()
{
	m_instancingManager = NewGO<InstancingManager>(0, "instancingManager");
	std::unordered_set<std::string> usedStageObjectNameSet;
	std::unordered_map<std::string, std::string> paths;
	m_coinManager->SetInstancingManager(m_instancingManager);
	m_bonusItemManager->SetInstancingManager(m_instancingManager);


	LoadScene("Assets/Scene/SceneExport.json", [&](const nlohmann::json& json)
		{
			//nlohmann::jsonはC++のライブラリでJSONデータを扱うためのものです。
			//nlohmann::jsonはstd::mapのように動く。

			//nameに対応する文字列を取得する。
			std::string name = json["name"];

			float scaleOffset = 10.0f / 5.0f;
			float posOffset = 390 / 5;

			//nameとtargetNameが等しいか調べる。
			//findはO(1)で済む。
			if (stageObjectNameSet.find(name) != stageObjectNameSet.end()) {

				//ParseTransformComponentsはtupleでposition, rotation, scaleをまとめた型のものを返す。
				auto transform = ParseTransformComponents(json["Transform"]);

				//トランスフォームからpos,rot,scaleにする
				Vector3 pos = Vector3::Zero;
				Quaternion rot;
				Vector3 scale;
				std::tie(pos, rot, scale) = transform;

				//トランスフォームを調整
				pos *= OBJECT_POS_OFFSET; // ポジション
				pos.y += -3000;
				scale *= OBJECT_SCALE_OFFSET; // スケール


				if (m_coinManager != nullptr)
				{
					//コインだったら
					if (name == "Coin") {
						m_coinManager->Spawn(pos, rot, scale);
						return true;
					}
				}


				if (m_bonusItemManager != nullptr)
				{
					// ボーナスアイテムだったら
					if (BonusItemManager::IsSupportedItemName(name))
					{
						BonusItemSpawnPoint spawnPoint = { name,pos };
						m_bonusItemManager->SetBonusItemSpawnPoint(spawnPoint);
						return true;
					}
				}

				if (name == "BonusItemWaypoint")
				{
					m_bonusItemWaypointSet.AddPoint(pos);
					return true;
				}

				//startポジションだったら
				if (name == "StartPosition")
				{
					m_playerStartPos = pos;
					return true;
				}

				if (name == "stuckRecoveryPos")
				{
					AssistWarpPoint assistWarpPoint;
					assistWarpPoint.position = pos;
					assistWarpPoint.rotation = rot;
					m_assistWarpPoint.push_back(assistWarpPoint);
					return true;
				}

				else
				{
					//実際に使われたオブジェクト名を保存しておく。
					usedStageObjectNameSet.insert(name);

					paths[name] = "Assets/modelData/stage/islandStage/" + name + ".tkm";

					auto* object = NewGO<StageMeshObject>(0);
					object->Init(paths[name].c_str(), pos, rot, scale, name.c_str());
					m_stageObjects.push_back(object);

					return true;
				}
			}
		});

	if (m_bonusItemManager != nullptr)
	{
		// Stageが所有するウェイポイント集合をManagerへ渡す。
		m_bonusItemManager->SetWaypointSet(
			&m_bonusItemWaypointSet
		);

		// 目標数まで初期アイテムを生成する。
		m_bonusItemManager->SpawnInitialItems();
	}

	std::unordered_map<std::string, bool> instancingFlags;
	std::unordered_map<std::string, size_t> maxInstanceTable;

	//インスタンシング描画に登録するためのデータを用意する。
	for (const auto& name : usedStageObjectNameSet) {

		//各種類のオブジェクトがインスタンシングするかを保存。
		instancingFlags[name] = PlacementObject::GetIsInstancing(name.c_str());

		//マックスインスタンスのテーブルを準備
		maxInstanceTable[name] = PlacementObject::GetMaxInstanceCount(name.c_str());
	}

	//インスタンシング描画に登録する。
	m_instancingManager->RegisterInstancingModels(
		usedStageObjectNameSet,
		paths,
		instancingFlags,
		maxInstanceTable);

	m_coinManager->RegisterCoinInstancingModel();


	if (m_bonusItemManager != nullptr)
	{
		m_bonusItemManager->RegisterBonusItemInstancingModels();
	}

	m_coinManager->ActivateInitialCoins();

}

bool Stage::Start()
{

	return true;
}
