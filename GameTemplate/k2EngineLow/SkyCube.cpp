#include "k2EngineLowPreCompile.h"
#include "SkyCube.h"
namespace nsK2EngineLow
{

	SkyCube::SkyCube()
	{
		m_textureFilePaths[enSkyCubeType_Day] = L"Assets/modelData/preset/skyCubeMap.dds";
		m_textureFilePaths[enSkyCubeType_Night] = L"Assets/modelData/preset/skyCubeMapNight.dds";
		m_textureFilePaths[enSkyCubeType_Snow] = L"Assets/modelData/preset/skyCubeMapSnow.dds";
		m_textureFilePaths[enSkyCubeType_Snow_2] = L"Assets/modelData/preset/skyCubeMapSnow_2.dds";
		m_textureFilePaths[enSkyCubeType_Wild] = L"Assets/modelData/preset/skyCubeMapWild.dds";
		m_textureFilePaths[enSkyCubeType_Wild_Night] = L"Assets/modelData/preset/skyCubeMapWild_Night.dds";
		m_textureFilePaths[enSkyCubeType_Wild_2] = L"Assets/modelData/preset/skyCubeMapWild_2.dds";
		m_textureFilePaths[enSkyCubeType_Grass] = L"Assets/modelData/preset/skyCubeMapGrass.dds";
		m_textureFilePaths[enSkyCubeType_Euro] = L"Assets/modelData/preset/skyCubeMapEuro.dds";
		m_textureFilePaths[enSkyCubeType_DayToon] = L"Assets/modelData/preset/skyCubeMapDay_Toon.dds";
		m_textureFilePaths[enSkyCubeType_DayToon_2] = L"Assets/modelData/preset/skyCubeMapDay_Toon_02.dds";
		m_textureFilePaths[enSkyCubeType_DayToon_3] = L"Assets/modelData/preset/skyCubeMapDay_Toon_03.dds";
		m_textureFilePaths[enSkyCubeType_DayToon_4] = L"Assets/modelData/preset/skyCubeMapDay_Toon_04.dds";
		m_textureFilePaths[enSkyCubeType_NightToon] = L"Assets/modelData/preset/skyCubeMapNight_Toon.dds";
		m_textureFilePaths[enSkyCubeType_NightToon_2] = L"Assets/modelData/preset/skyCubeMapNight_Toon_02.dds";
		m_textureFilePaths[enSkyCubeType_SunriseToon] = L"Assets/modelData/preset/skyCubeMapSunrise_Toon.dds";
		m_textureFilePaths[enSkyCubeType_SpaceToon] = L"Assets/modelData/preset/skyCubeMapSpace_Toon.dds";
		m_textureFilePaths[enSkyCubeType_SpaceToon_2] = L"Assets/modelData/preset/skyCubeMapSpace_Toon_2.dds";

		for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); ++i) {
			auto layer = static_cast<ReflectLayer>(i);
			m_reflectModelRender.try_emplace(layer);
			m_skyCB.try_emplace(layer);
		}


	}

	SkyCube::~SkyCube()
	{

	}

	bool SkyCube::Start()
	{
		
		InitModel();

		for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); ++i) {
			auto layer = static_cast<ReflectLayer>(i);
			InitReflectModel(layer);
			InitSkyCB(layer);
		}
		return true;
	}

	void SkyCube::Update()
	{
		for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); ++i) {
			auto layer = static_cast<ReflectLayer>(i);
			UpdateSkyCB(layer);
		}

		if (m_isDirty)
		{
			m_modelRender.SetTransform(m_position, m_rot, m_scale);
			m_modelRender.Update();

			for (int i = 0; i < static_cast<int>(ReflectLayer::enNum); ++i) {
				auto layer = static_cast<ReflectLayer>(i);
				m_reflectModelRender[layer].SetTransform(m_position, m_rot, m_scale);
				m_reflectModelRender[layer].Update();
			}
			m_isDirty = false;


		}
	}

	void SkyCube::Render(RenderContext& rc)
	{
		m_modelRender.Draw(rc);
		m_reflectModelRender[ReflectLayer::enOcean].Draw(rc);
	}

	void SkyCube::InitModel()
	{
		ModelInitData initData;
		//tkmファイルのファイルパスを指定する。
		initData.m_tkmFilePath = "Assets/modelData/preset/sky.tkm";
		//シェーダーファイルのファイルパスを指定する。
		initData.m_fxFilePath = "Assets/shader/skyCube.fx";
		initData.m_vsEntryPointFunc = "VSMain";
		initData.m_psEntryPointFunc = "PSMain";

		for (int i = 0; i < enSkyCubeType_Num; i++) {
			m_texture[i].InitFromDDSFile(m_textureFilePaths[i]);
		}

		initData.m_expandShaderResoruceView[0] = &m_texture[m_type];
		initData.m_expandConstantBuffer = &m_luminance;
		initData.m_expandConstantBufferSize = sizeof(m_luminance);
		m_modelRender.InitSkyCubeModel(initData);
		m_modelRender.SetTransform(m_position, m_rot, m_scale);
		m_modelRender.Update();
	}

	void SkyCube::InitReflectModel(ReflectLayer layer)
	{
		ModelInitData initData;
		//tkmファイルのファイルパスを指定する。
		initData.m_tkmFilePath = "Assets/modelData/preset/sky.tkm";
		//シェーダーファイルのファイルパスを指定する。
		initData.m_fxFilePath = "Assets/shader/skyCube.fx";
		initData.m_vsEntryPointFunc = "VSReflectionMain";
		initData.m_psEntryPointFunc = "PSMain";

		//for (int i = 0; i < enSkyCubeType_Num; i++) {
		//	m_texture[i].InitFromDDSFile(m_textureFilePaths[i]);
		//}

		initData.m_expandShaderResoruceView[0] = &m_texture[m_type];
		initData.m_expandConstantBuffer = &m_skyCB[layer];
		initData.m_expandConstantBufferSize = sizeof(m_skyCB[layer]);
		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_reflectModelRender[layer].InitSkyCubeReflectionModel(initData);
		m_reflectModelRender[layer].SetTransform(m_position, m_rot, m_scale);
		m_reflectModelRender[layer].Update();
	}
}