#pragma once
class SceneManager;
class IScene :public IGameObject
{
public:
	IScene();
	virtual ~IScene();
	virtual bool Start() override = 0;
	virtual void Update() final;
	virtual void Render(RenderContext& rc) override = 0;
	virtual void Init(SceneManager* sceneManager) final;


public:

	/// <summary>
	/// このシーンに入ったときに呼ばれる
	/// 子で何も書くことがないことがあるかもしれないので
	/// デフォルト実装を用意しておく
	/// </summary>
	void OnEnter() {};

	/// <summary>
	/// このシーンから出るときに呼ばれる
	/// 子で何も書くことがないことがあるかもしれないので
	/// デフォルト実装を用意しておく
	/// </summary>
	void OnExit() {};

	/// <summary>
	/// シーンマネージャーの依存注入
	/// </summary>
	/// <param name="sceneManager"></param>
	void SetSceneManager(SceneManager* sceneManager) {
		m_sceneManager = sceneManager;
	}

private:

	virtual void OnInit() {};

	/// <summary>
	/// 更新処理
	/// 派生クラスではこっちを使う。
	/// </summary>
	virtual void OnUpdate() = 0;

	/// <summary>
	/// sceneを遷移するかチェックする
	/// </summary>
	virtual void CheckChangeScene() = 0;


protected:
	SceneManager* m_sceneManager = nullptr;
};

