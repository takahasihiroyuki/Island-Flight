#pragma once

class StageMeshObject;
class CoinManager;
class Stage :public IGameObject
{
public:
	Stage() {
		float debug = 0;
	};
	~Stage() {
		DeleteGO(m_instancingManager);

	};

	bool Start() override;
	void GetCoinManager(CoinManager* coinManager)
	{
		m_coinManager = coinManager;
	}
	void Update() {
	}

private:
	CoinManager* m_coinManager;
	InstancingManager* m_instancingManager = nullptr;
	Vector3 m_posOfset = Vector3::Zero;
	Vector3 m_ScaleOfset;

};

