#pragma once
class Game : public IGameObject
{
public:

	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
private:
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V�����A
		enAnimClip_Run,		// 1 : ����A�j���[�V�����B
		enAnimClip_Num,		// 2 :�A�j���[�V�����N���b�v�̐��B
	};

	ModelRender m_model;					// ���f��
	Vector3 m_position;				// ���W
	Quaternion m_rotation;			// ��]
	Vector3 m_scale = g_vec3One;	// �g�嗦
	std::array<AnimationClip, 4> m_animationClipArray;	// �A�j���[�V�����N���b�v�z��
};

