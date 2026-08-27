#pragma once
class SoundManager :public Noncopyable
{
public:
	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}
	void Update();
	void Init();
	void Shutdown();


private:
	SoundManager() = default;


private:
};

