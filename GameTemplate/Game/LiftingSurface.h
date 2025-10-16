#pragma once
#include "AircraftState.h"
///�G�A�[�N���t�g�i��s�@�j�N���X�������N���X
///�ЂƂ̗��͎嗃�����Ƒǖ�(ControlSurface)�i�������ē��͂ɂ���ē������镔���j�������Ă���
/// �G�A�[�|�[�g�N���X�ł͂��̃N���X�̃C���X�^���X���O���B

/// <summary>
/// �ǖ�
/// ���̓��͂ɂ���ē��������
/// </summary>
struct ControlSurface
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="maxDef">�Ȃ���ő�̊p�x�i���W�A���j</param>
	void Init(float maxDef) {
		maxDeflection = maxDef;
	}

	void SetAreaRatio(float ratio) { areaRatio = ratio; }
	void SetControlInput(float input)
	{
		if (input >= 1) {
			controlInput = 1; 
			return;
		}

		if (input <= 0)
		{
			controlInput = -1;
			return;
		}

		controlInput = input;
	}
	void SetDeflection(float def) { deflection = def; }

	void UpdateDeflection() {
		deflection = controlInput * maxDeflection;
	}

	float GetAreaRatio() const { return areaRatio; };
	float GetDelection() const { return deflection; };
	float GetMaxDeflection()const { return maxDeflection; };

private:
	float areaRatio = 0.25;//���̖ʂ������߂銄��
	float controlInput = 0.0f;   // ���Ǔ��́i�X�e�B�b�N�Ȃǁj
	float deflection = 0.0f;     //���ۂ̑Ǌp�i���l�I�ǖʂ̓����j�i���W�A���j
	float maxDeflection = 0.0f;
};


class AircraftState;
class LiftingSurface
{
public:
	LiftingSurface() {};
	LiftingSurface(
		Quaternion orientation,
		bool isMirroed,
		float maxWingDeflectionAngle,
		Vector3 momentArm,
		bool isVertical = false
	);
	~LiftingSurface();
	//void Render(RenderContext& rc) override;
	bool Start();

public:
	void UpdateControlSurface() {
		m_controlSurface.UpdateDeflection();
	}

	void SetControlInput(float input) {
		m_controlSurface.SetControlInput(input);
	}

	Vector3 GetForce() const {
		return m_force;
	}

	void ComputeForces(const AircraftState& state);

	/// <summary>
	/// ���[�����g���v�Z
	/// ���[�����g�̌������񂷎�
	/// �傫��������
	/// ComputeForces��ǂ񂾌�ɌĂ�
	/// </summary>
	/// <returns></returns>
	Vector3 ComputeMoment() {
		Vector3 moment;
		moment.Cross(m_worldMomentArm, m_force);
		return moment;
	};

	void InitOrientation(const Quaternion& rot, bool isMirrored, bool isVertical)
	{
		m_localChordDir = Vector3::Back;
		m_localNormalDir = Vector3::Up;
		m_localSpanDir = Vector3::Right;

		if (isVertical) {
			// ���������̊��
			m_localChordDir = Vector3::Back; // ���͂��̂܂ܑO��
			m_localNormalDir = Vector3::Right; // �@���� +X ��
			m_localSpanDir = Vector3::Up;    // �X�p���� +Y ��
		}

		// ��]��K�p���ė����������[���h��Ԃɕϊ�
		rot.Apply(m_localChordDir);
		rot.Apply(m_localNormalDir);
		rot.Apply(m_localSpanDir);

		if (isMirrored)
			m_localSpanDir *= -1.0f;
	}

	void UpdateOrientation(Quaternion orientation);
private:

	/// <summary>
	/// ���Ε���wingChordDir�̊p�x�i�}�p�j���v�Z����B
	/// </summary>
	/// <param name="m_relWind"></param>
	float ComputeAngleOfAttack(const Vector3& relWind);

	float ComputeDynamicPressure(const AircraftState& state);

	/// <summary>
	/// �g�́i�@�������ɓ����́j���v�Z����B
	/// </summary>
	/// <param name="angleOfAttack">�}�p</param>
	Vector3 ComputeLift(
		const AircraftState& state,
		float dynamicPressure,
		float angleOfAttack);

	Vector3 ComputeDrag(
		const AircraftState& state,
		float dynamicPressure,
		float angleOfAttack);

	/// <summary>
	/// �}�p����g�͌W�����v�Z����B
	/// �^����ꂽ�}�p�ɑ΂��āA���̗����ǂ̒��x�����I�ɗg�͂𔭐��ł��邩�������W����Ԃ��B
	/// 
	/// TODO: ���݂͊ȈՃ��f�����g�p���Ă��邪�A
	/// ���ۂ̗g�͌W���f�[�^�𗘗p������ԃ��f���ɂ��u�������\�B
	/// NOTE: ���̊֐��͌����Ȑ��x��K�v�Ƃ��Ȃ����߁A����̋ߎ��ł��\���B
	/// </summary>
	/// <param name="angleOfAttack">�}�p</param>
	/// <returns>�g�͌W��</returns>
	float ComputeLiftCoefficient(float angleOfAttack) const;

	/// <summary>
	/// �R�͌W��
	/// �}�p����R�͌W�����v�Z����B
	/// �g�͌W���ƍR�͌W���̊֌W�����g����
	/// �g�͌W������v�Z����B
	/// </summary>
	/// <param name="angleOfAttack"></param>
	/// <returns></returns>
	float ComputeDragCoefficient(float angleOfAttack) const;

	


private:

	Vector3 m_position = Vector3(0.0f, 0.0f, 0.0f);
	Quaternion m_rot = g_quatIdentity;


	Vector3 m_wingChordDir = Vector3::Back; // ���̑O��������֐L�΂����x�N�g���̕����i�O������j
	Vector3 m_wingSpanDir;					// ���̍��{���痃�[�֐L�΂����x�N�g���̕����i���E�����j
	Vector3 m_wingNormal = Vector3::Up;		// ���̖@��
	Vector3 m_localChordDir ;
	Vector3 m_localSpanDir ;
	Vector3 m_localNormalDir;
	Vector3 m_force;						// ���ɓ����́i�g�́{�R�́j
	float m_area = 1.0f;				// ���ʐ�
	float m_airDensity = 1.225f;		// ��C���x (kg/m^3) �W����C�����ł̒l
	ControlSurface m_controlSurface;
	Vector3 m_localMomentArm;				//���[�����g���v�Z���邽�߂́i�d�S���痃�ւ̋����j
	Vector3 m_worldMomentArm;
};

