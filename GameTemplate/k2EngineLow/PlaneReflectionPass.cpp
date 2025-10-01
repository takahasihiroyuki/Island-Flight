#include "k2EngineLowPreCompile.h"
#include "PlaneReflectionPass.h"

namespace nsK2EngineLow
{
    PlaneReflectionPass::PlaneReflectionPass()
    {
    }

    PlaneReflectionPass::~PlaneReflectionPass()
    {
    }

    void PlaneReflectionPass::Init()
    {
        m_planarReflectionTarget.Create(
            g_graphicsEngine->GetFrameBufferWidth(),
            g_graphicsEngine->GetFrameBufferHeight(),
            1,
			1,
            DXGI_FORMAT_R11G11B10_FLOAT,
            DXGI_FORMAT_D24_UNORM_S8_UINT
        );
    }

    void PlaneReflectionPass::Execute(RenderContext& rc, std::vector<ModelRender*>& obj)
    {
        rc.WaitUntilToPossibleSetRenderTarget(m_planarReflectionTarget);
        rc.SetRenderTargetAndViewport(m_planarReflectionTarget);
        rc.ClearRenderTargetView(m_planarReflectionTarget);
        rc.ClearDepthStencilView(m_planarReflectionTarget.GetDSVCpuDescriptorHandle(), 1.0f);
        
        // �܂Ƃ߂ă����ʂɉf���f�������_�[��`��
        for (auto MobjData : obj)
        {
            //���ʂ̕`��
            MobjData->OnRenderReflectionMap(rc, m_reflectCamera);
        }

        rc.WaitUntilFinishDrawingToRenderTarget(m_planarReflectionTarget);
    }

    void PlaneReflectionPass::UpdateReflectCamera(const Plane& plane)
    {
        //���C���J�����̏ꏊ�����f�������ʒu�ɔ��˃J������u��
        m_reflectCamera.SetPosition(ReflectPointAcrossPlane(g_camera3D->GetPosition(), plane));

        m_reflectCamera.SetTarget(ReflectPointAcrossPlane(g_camera3D->GetTarget(), plane));

        m_reflectCamera.SetUp(ReflectVectorAcrossPlane(g_camera3D->GetUp(), plane));

        // �΂߃N���b�v�ʂ��r���[��Ԃ�
        Plane planeVS = TransformPlaneToView(plane, m_reflectCamera.GetViewMatrix());

        //// �ʂ��N���b�v�ʂƂ��Ďg���悤�Ȏˉe�s����쐬
        //m_reflectCamera.SetProjectionMatrix(MakeObliqueProjectionLike(g_camera3D->GetProjectionMatrix(), planeVS));

        m_reflectCamera.SetProjectionMatrix(g_camera3D->GetProjectionMatrix());
    }

    Plane PlaneReflectionPass::TransformPlaneToView(const Plane& planeWS, const Matrix& view)
    {
        //���[���h��ԁ��J������Ԃɕ��ʂ�ϊ�����
        //���̍ہA�@���𐳂����ϊ����邽�߂�view�s��̋t�]�u���g�p����
        Matrix viewInvTranspose;
        viewInvTranspose.Inverse(view);
        viewInvTranspose.Transpose();

        // planeWS�̕�������view��Ԃɕϊ�
        Vector4 planeEquationVS = planeWS.GetEquation();
        viewInvTranspose.Apply(planeEquationVS);

        // ���K�����ꂽ�r���[��Ԃ̕��ʂ𐶐�
        Plane  planeVS = Plane::FromEquationNormalized(planeEquationVS);

        // �΂߃j�A�ʂƂ��Ďg���O��ŁA���_�������i�ʏ�͕����j�ɂȂ�悤�����𓝈�
        if (planeVS.GetSignedDistance() > 0.0f) {
            planeVS = Plane(planeVS.GetNormal() * -1, planeVS.GetSignedDistance() * -1);
        }


        return planeVS;
    }

    Matrix PlaneReflectionPass::MakeObliqueProjectionLike(const Matrix& proj, const Plane& planeVS)
    {

        // proj�̋t�s���p���uclip-space �̂ǂ̓_�������邩�����߂�
        Vector4 q;
        q.x = (std::copysign(1.0f, planeVS.GetEquation().x) + proj._13) / proj._11;
        q.y = (std::copysign(1.0f, planeVS.GetEquation().y) + proj._23) / proj._22;
        q.z = 1.0f;
        q.w = (1.0f - proj._33) / proj._34;

        // �V�����s���v�Z
        Vector4 c = planeVS.GetEquation() * (2.0f / Dot(planeVS.GetEquation(), q));

        Matrix obliqueProj = proj;
        obliqueProj.SetRow3(c - proj.GetRow4());  // ��3�s�������ւ���
        return obliqueProj;
    }

}