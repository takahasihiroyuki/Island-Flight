#pragma once
#include "SceneType.h"
namespace {
	const SceneType INIT_SCENE = SceneType::Title;
}


class IScene;
class SceneManager :public IGameObject
{
public:

	SceneManager();
	~SceneManager();
	bool Start() override;
	void Update() override;

private:

	/// <summary>
	///	Ÿ‚ÌƒV[ƒ“‚ÖˆÚ‚éB
	/// </summary>
	/// <param name="type"></param>
	void ChangeScene(SceneType type);


private:

	bool m_requestPending = false;
	SceneType m_nextSceneType = INIT_SCENE;
	IScene* m_currentScene = nullptr;

};

