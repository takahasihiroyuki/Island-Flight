#include "stdafx.h"
#include "Game.h"
#include "Aircraft.h"
#include"PlacementObject.h"
#include"CoinManager.h"
#include "Coin.h"
#include "ScoreManager.h"
#include "level3D/Level.h"


namespace
{
	bool ForwardMatchName(const wchar_t* name, const wchar_t* n)
	{
		auto len = wcslen(n);
		auto namelen = wcslen(name);
		if (len > namelen) {
			//名前が長い。不一致。
			return false;
		}
		return wcsncmp(n, name, len) == 0;
	}

	std::string FindAssetPath(const wchar_t* name)
	{
		if (ForwardMatchName(name, L"Bush")) {
			return "Assets/modelData/Plane/Plane.tkm";
		}
		if (ForwardMatchName(name, L"StaticMesh_Mounten")) {
			return "Assets/modelData/Plane/Plane.tkm";
		}
		if (ForwardMatchName(name, L"StaticMesh_Mounten")) {
			return "Assets/modelData/unityChan.tkm";
		}
	}

}


Game::Game()
{
	for (int i = 0; i < 5; i++) {
		m_model.Init("Assets/modelData/Plane/Plane.tkm", nullptr, 0, enModelUpAxisZ, false);
	}
	m_bg.Init("Assets/modelData/bg/bg.tkm", nullptr, 0, enModelUpAxisZ, true);
	m_island.Init("Assets/modelData/stage/islandStage/BananaTree_1.tkm", nullptr, 0, enModelUpAxisZ, true);



}

Game::~Game()
{
}

bool Game::Start()
{
	m_island.SetPosition({ 300.0f,0.0f,2000.0f });

	m_island.Update();

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetScale(10000.0f);
	m_skyCube->SetPosition({ 0.0f,0.0f,0.0f });
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);

	m_ocean = NewGO<Ocean>(0);
	m_aircraft = NewGO<Aircraft>(0, "aircraft");
	m_aircraft->Init("Assets/modelData/Plane/Plane.tkm");
	m_coinManager = NewGO<CoinManager>(0, "coinManager");
	m_scoreManager = NewGO<ScoreManager>(0, "ScoreManager");

	m_coinManager->SetScoreManager(m_scoreManager);

	nsK2EngineLow::Level level3D;

	level3D.Init("Assets/level/stageTest.tkl", [&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"StaticMesh"))
			{
				auto* object = NewGO<StageMeshObject>(0);
				object->Initialize(FindAssetPath(objData.name).c_str(), objData.position, objData.rotation, objData.scale);

				m_placementObject.push_back(object);
				return true;
			}
			if (objData.ForwardMatchName(L"Bush"))
			{
				CoinDesc desc;
				desc.pos = objData.position + Vector3::Right * 200 + Vector3::Front*20.0f;
				desc.rot = objData.rotation;
				desc.scale = objData.scale;
				m_coinManager->Spawn(desc);
				return true;
			}
			if (objData.ForwardMatchName(L"Coin"))
			{
				//CoinDesc desc;
				//desc.pos = objData.position;
				//desc.rot = objData.rotation;
				//desc.scale = objData.scale;
				//desc.modelPath = FindAssetPath(objData.name);

				//m_coinManager->Spawn(desc);

			}



			return true;
		});

	return true;
}
void Game::Update()
{
	m_coinManager->Update(*m_aircraft);

	// 左スティック(キーボード：WASD)で平行移動。
	//m_position.y += g_pad[0]->GetLStickXF()*100.0;
	//m_position.z += g_pad[0]->GetLStickYF()*100.0f;


	// 右スティック(キーボード：上下左右)で回転。
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	m_cameraPosition.x += g_pad[0]->GetRStickXF() * 5;
	m_cameraPosition.y += g_pad[0]->GetRStickYF() * 5;



	g_camera3D->SetPosition(m_aircraft->GetPosition() + Vector3(0.0f, 300.0f, -500.0f));
	g_camera3D->SetTarget(m_aircraft->GetPosition() + Vector3(0.0f, 100.0f, 0.0f));
	m_skyCube->SetPosition(Vector3(m_position.x,0.0f,m_position.z));


	for (int i = 0; i < 5; i++) {
		m_model.SetPosition(m_position /*+ Vector3{ (float)(i - 2) * 50.0f,0.0f,0.0f }*/);

		m_model.SetRotation(m_rotation);
		m_model.Update();
	}

	g_camera3D->SetFar(100000);
}
void Game::Render(RenderContext& rc)
{
	m_skyCube->Render(rc);
	/*m_island.Draw(rc);*/
}