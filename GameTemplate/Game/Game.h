#pragma once
#include "SkyCube.h"

class Aircraft;
class Game : public IGameObject
{
public:

	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	/// <summary>
	/// �_���w�肵�����ʂɑ΂��ċ��f�������ʒu��Ԃ��B
	/// </summary>
	/// <param name="point">���˂��������_</param>
	/// <param name="plane">����</param>
	/// <returns></returns>
	inline Vector3 ReflectPointAcrossPlane(const Vector3& point, const Plane& plane) {
		// ���ʂ܂ł̕����t���������v�Z�i�@���͒P�ʒ��̑O��j
		float dist = plane.SignedDistanceTo(point);

		return point - (plane.GetNormal() * dist * 2.0f);
	}

	/// <summary>
	/// �x�N�g���𕽖ʂɑ΂��Ĕ��˂�����B
	/// </summary>
	/// <param name="v">���˂������������x�N�g��</param>
	/// <param name="plane">���ʁi�@���ƃI�t�Z�b�g�������A�x�N�g�����˂ł͖@�������g���j</param>
	/// <returns>���ˌ�̃x�N�g��</returns>
	inline Vector3 ReflectVectorAcrossPlane(const Vector3& vector, const Plane& plane)
	{
		const Vector3& n = plane.GetNormal();   // �@���x�N�g��

		// �ˉe�W���in���������̑傫�� / |n|^2�j
		const float proj = Dot(n, vector);

		return vector - (n * proj * 2);
	}

private:
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V�����A
		enAnimClip_Run,		// 1 : ����A�j���[�V�����B
		enAnimClip_Num,		// 2 :�A�j���[�V�����N���b�v�̐��B
	};

	ModelRender m_model;					// ���f��
	ModelRender m_bg;				// �w�i���f��
	ModelRender m_island;			// �����f��
	Vector3 m_position = Vector3{0.0,30.0f,100.0f};				// ���W
	Vector3 m_cameraPosition=Vector3::Zero;		// �J�������W
	Vector3 m_cameraTarget;			// �J���������_
	Quaternion m_rotation;			// ��]
	Vector3 m_scale = g_vec3One;	// �g�嗦
	std::array<AnimationClip, 4> m_animationClipArray;	// �A�j���[�V�����N���b�v�z��
	Plane plane = Plane(Vector3{0.0f,1.0f,0.0f}, 0.0f); // ������
	Ocean* m_ocean = nullptr;
	bool m_cameraFlag = false;
	float m_timer = 0.0f;
	SkyCube* m_skyCube = nullptr;
	CharacterController m_characterController;
	Vector3 m_moveSpeed = Vector3::Zero;
	float angle = 0.0f;
	Aircraft* m_aircraft = nullptr;
};

