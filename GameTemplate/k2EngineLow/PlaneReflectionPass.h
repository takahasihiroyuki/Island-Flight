#pragma once
namespace nsK2EngineLow
{
	/// <summary>
	/// ���ʂ̕�������\���\���́B
	/// ���ʂ������₷���悤�ɕ������̌`�ɂ��Ă���B
	/// ���ʂ̕������Fax + by + cz + d = 0
	/// P�oa, b, c�p�̓��[���h���W�̔C�ӂ̓_
	/// 
	/// </summary>
	struct Plane {

	public:
		/// <summary>
		/// �@���x�N�g���ƕ��ʏ�̔C�ӂ̓_���w�肵�ĕ��ʂ𐶐�����B
		/// </summary>
		/// <param name="n">���ʂ̖@��</param>
		/// <param name="p">���ʏ�̓_</param>
		Plane(const Vector3& n, const Vector3& p)
			: normal(n)
		{
			//n��0�x�N�g�����m���߂邽�߂̕ϐ�
			//�iLength()�Œ��������߂�ƁASqrt()���g���̂ŁADot()�̕��������������j
			const float nn = Dot(n, n);
			if (nn < 1e-12f) {
				//n��0�x�N�g���ɋ߂��Ƃ��́AY��������@���Ƃ���
				normal = Vector3{ 0,1,0 };
			}
			normal.Normalize();
			equation = Vector4(normal.x, normal.y, normal.z, -Dot(normal, p));
			signedDistance = equation.w; // D�̕����𔽓]���������̂����_����̕����t������
		}

		/// <summary>
		/// ���ʂ̕�������d���w�肵�ĕ��ʂ𐶐�����B
		/// </summary>
		/// <param name="n">���ʂ̖@��</param>
		/// <param name="d">���ʂ̕�������D�B���_���畽�ʂ܂ł̕����t�������in ���P�ʃx�N�g���̂Ƃ��j</param>
		Plane(const Vector3& n, float d)
			: normal(n), signedDistance(d)
		{
			normal.Normalize();
			equation = Vector4(normal.x, normal.y, normal.z, d);
		}


		/// <summary>
		/// ���ʕ������̌W��(a,b,c,d)����Plane������B
		/// �@��(a,b,c) �͐��K������AD �������X�P�[���Ŋ����邽�߁A
		/// ���ʂƂ��ĒP�ʖ@���������K���ς݂̕��ʂ�������B
		/// </summary>
		/// <param name="eq"></param>
		/// <returns></returns>
		static Plane FromEquationNormalized(const Vector4& eq) {
			Vector3 n{ eq.x, eq.y, eq.z };
			float len = n.Length();
			if (len < 1e-12f) {
				return Plane(Vector3{ 0,1,0 }, 0.0f);
			}
			n /= len;
			float d = eq.w / len;
			return Plane(n, d);
		}

		/// <summary>
		/// �C�ӂ̓_�Ƃ��̕��ʂ̕����t��������Ԃ��B
		/// </summary>
		/// <param name="p">�C�ӂ̓_</param>
		/// <returns></returns>
		float SignedDistanceTo(const Vector3& p) const {
			//�@�������K������Ă���O��B
			//�R���X�g���N�^�ŕK�����K������悤�ɂ��Ă���̂Ŗ��Ȃ��B
			return Dot(normal, p) + signedDistance;
		}

		const Vector3& GetNormal() const { return normal; }
		const Vector4& GetEquation() const { return equation; }
		float GetSignedDistance() const { return signedDistance; }

	private:
		Vector3 normal;       // �@���x�N�g��
		Vector4 equation;     // ���ʂ̕����� Ax + By + Cz + D = 0 �̌W�� (A, B, C, D)
		float signedDistance; // ���_���畽�ʂ̕����t���ŒZ����
	};

	class PlaneReflectionPass
	{
	public:
		PlaneReflectionPass();
		~PlaneReflectionPass();
		void Init();
		void Execute(RenderContext& rc, std::vector<ModelRender*>& obj);

		/// <summary>
		/// ���˃J�������X�V����B
		/// </summary>
		/// <param name="plane"></param>
		void UpdateReflectCamera(const Plane& plane);

		Camera& GetReflectCamera() {
			return m_reflectCamera;
		}

		RenderTarget& GetPlanarReflectionTarget() {
			return m_planarReflectionTarget;
		}

		Matrix GetReflectViewMatrix() {
			return m_reflectCamera.GetViewMatrix();
		}

	private:
		/// <summary>
		/// �_���w�肵�����ʂɑ΂��ċ��f�������ʒu��Ԃ��B
		/// </summary>
		/// <param name="point">���˂��������_</param>
		/// <param name="plane">����</param>
		/// <returns></returns>
		Vector3 ReflectPointAcrossPlane(const Vector3& point, const Plane& plane) {
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
		Vector3 ReflectVectorAcrossPlane(const Vector3& vector, const Plane& plane)
		{
			const Vector3& n = plane.GetNormal();   // �@���x�N�g��

			// �ˉe�W���in���������̑傫�� / |n|^2�j
			const float proj = Dot(n, vector);

			return vector - (n * proj * 2);
		}

		/// <summary>
		/// ���[���h��Ԃ̕��ʂ��r���[��Ԃɕϊ�����B
		/// </summary>
		/// <param name="planeWS"></param>
		/// <param name="V"></param>
		/// <returns></returns>
		Plane TransformPlaneToView(const Plane& planeWS, const Matrix& V);

		/// <summary>
		/// �^����ꂽ�ˉe�s������ƂɁA�΂߃N���b�v�ʂ����ˉe�s����쐬����B
		/// </summary>
		/// <param name="proj"></param>
		/// <param name="planeVS"></param>
		/// <returns></returns>
		Matrix MakeObliqueProjectionLike(const Matrix& proj, const Plane& planeVS);

	private:
		RenderTarget m_planarReflectionTarget;
		float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };	//�J���[�o�b�t�@�[�͐^����
		Camera m_reflectCamera;                         // ���˗p�J����

	};
}

