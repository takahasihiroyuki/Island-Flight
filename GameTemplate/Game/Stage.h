#pragma once

class StageMeshObject;
class CoinManager;
class Stage:public IGameObject
{
public:
	Stage() {};
	~Stage() {};

	bool Start() override;


private:
	CoinManager* m_coinManager;
	std::vector<StageMeshObject*> m_stageMeshObject;
};

