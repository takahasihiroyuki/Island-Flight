#include "stdafx.h"
#include "Stage.h"
#include "SceneLoader/SceneLoader.h"
#include <vector>
#include <string>
#include <unordered_set>
#include"CoinManager.h"
#include "Coin.h"
#include "PlacementObject.h"


namespace {

	const std::array<std::string, 137> objectNames =
	{
		//"BananaTree_1",
		//"BananaTree_2",
		//"BananaTree_3",
		//"Barrel_1",
		//"Block_Bridge",
		//"Block_L_1",
		//"Block_L_2",
		//"Block_L_3",
		//"Block_S_1",
		//"Block_S_2",
		//"Block_S_3",
		//"Block_T",
		//"Bones_Head",
		//"Bones_Skull",
		//"Bones_Torso",
		//"Bridge",
		//"Bucket_1",
		//"Bucket_2",
		//"Canon",
		//"CanonBall",
		//"CanonBalls",
		//"Cart",
		//"Cart_Broken",
		//"Column_Egypt_2",
		//"Column_Egypt_3",
		//"Column_V1_1",
		//"Column_V1_2",
		//"Column_V1_3",
		//"Column_V1_4",
		//"Column_V2_1",
		//"Column_V2_2",
		//"Column_V2_3",
		//"Crane_1",
		//"Crane_2",
		//"Crane_3",
		//"Crate_1",
		//"Crate_2",
		//"Elephant_Statue",
		//"Fence_1",
		"Fence_2",
		"Fence_3",
		"Fence_4",
		"Footbridge",
		//"Island_1",
		//"Island_2",
		//"Island_3",
		//"Ladder",
		//"Ladder_Broken",
		//"Palm_1",
		//"Palm_2",
		//"Palm_3",
		//"Palm_4",
		//"Palm_5",
		//"Palm_6",
		//"Palm_7",
		//"Palm_8",
		//"Palm_9",
		//"Palm_10",
		//"Plant_1",
		//"Plant_2",
		//"Plant_3",
		//"Plant_4",
		//"Plant_5",
		//"Plant_6",
		//"Plant_7",
		//"Plant_8",
		//"Plant_9",
		//"Plant_10",
		//"Pyramid_L",
		//"Pyramid_M",
		//"Pyramid_S",
		//"Rock_1",
		//"Rock_2",
		//"Rock_3",
		//"Rock_4",
		//"Rock_5",
		//"Rock_6",
		//"Rock_7",
		//"Rock_8",
		//"Rock_9",
		//"Rock_10",
		////"Rock_Formation_1",
		//"Shipwreck",
		//"Spear",
		//"Stall_1",
		//"Stall_2",
		//"Stall_3",
		//"Stone_1",
		//"Stone_2",
		//"Stone_3",
		//"Tent_1",
		//"Tent_2",
		//"Tent_3",
		//"Tile_1",
		//"Tile_2",
		//"Tile_3",
		//"Tile_4",
		//"Tile_5",
		//"Tile_6",
		//"Tile_7",
		//"Tile_8",
		//"Treasure_Chest",
		//"Vase_L_1",
		//"Vase_L_2",
		//"Vase_L_3",
		//"Vase_M_1",
		//"Vase_M_2",
		//"Vase_M_3",
		//"Vase_S_1",
		//"Vase_S_2",
		//"Vase_S_3",
		//"WaterVase_1",
		//"WaterVase_2",
		//"WaterVase_3",
		//"WodenLog",
		//"WodenLog_2",
		//"Wooden_Box"
	};
	//各オブジェクトごとに objectNames を線形探索すると全体で O(N²) になるため
	//高速に存在判定できる unordered_set に変換しておく。

	// 名前の集合を作成。
	const std::unordered_set<std::string> stageObjectNameSet(objectNames.begin(), objectNames.end());

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
		for (const auto& target : objectNames) {
			if (IsJsonStringEqual(json, target.c_str())) {
				return true;
			}
		}
		return false;
	}
};


bool Stage::Start()
{
	m_coinManager = FindGO<CoinManager>("coinManager");
	static int count = 0;
	LoadScene("Assets/Scene/SceneExport.json", [&](const nlohmann::json& json)
		{
			//nlohmann::jsonはC++のライブラリでJSONデータを扱うためのものです。
			//nlohmann::jsonはstd::mapのように動く。

			//nameに対応する文字列を取得する。
			const std::string name = json["name"];


				//ParseTransformComponentsはtupleでposition, rotation, scaleをまとめた型のものを返す。
				auto transform = ParseTransformComponents(json["Transform"]);

				if (name == "Coin") {
					CoinDesc desc;
					std::tie(desc.pos, desc.rot, desc.scale) = transform;
					m_coinManager->Spawn(desc);
					return true;
				}

				//nameとtargetNameが等しいか調べる。
				//findはO(1)で済む。
				if (stageObjectNameSet.find(name) != stageObjectNameSet.end()) {
					auto* object = NewGO<StageMeshObject>(0);
					std::string path = "Assets/modelData/stage/islandStage/" + name + ".tkm";
					std::get<0>(transform) = Vector3::One*100; // スケールは1固定
					object->Initialize(path.c_str(), transform);
					m_stageMeshObject.push_back(object);

					count++;

					return true;
				}
		});
	return true;

}
