#pragma once
class GameStateBase:public IGameObject
{
	virtual ~GameStateBase() {}
	virtual void Enter() {}
	virtual void Update() override {};
	virtual void Exit() {}
};

