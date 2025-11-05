#pragma once

class StageMeshObject;
class CoinManager;
class Stage :public IGameObject
{
public:
	Stage() {};
	~Stage() {};

	bool Start() override;
	void GetCoinManager(CoinManager* coinManager)
	{
		m_coinManager = coinManager;
	}

private:
	CoinManager* m_coinManager;
	InstancingManager* m_instancingManager = nullptr;
};

