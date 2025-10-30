#pragma once
enum class SceneType
{
	Title,
	InGame,
	GameResolt
};

namespace {
	const SceneType INIT_SCENE = SceneType::Title;
}


class IScene;
class SceneManager:public IGameObject
{
public:

	SceneManager();
	~SceneManager();
	bool Start() override;
	void Update() override;


public:

	void RequestChange(SceneType type)
	{
		m_nextSceneType = type;
		m_requestPending = true;
	}


private:

	/// <summary>
	///	Ÿ‚ÌƒV[ƒ“‚ÖˆÚ‚éB
	/// </summary>
	/// <param name="type"></param>
	void ChangeScene(SceneType type);


private:

	bool m_requestPending = false;
	SceneType m_nextSceneType= INIT_SCENE;
	IScene* m_currentScene=nullptr;

};

