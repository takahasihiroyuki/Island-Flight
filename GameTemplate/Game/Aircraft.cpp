#include "stdafx.h"
#include "Aircraft.h"
#include "LiftingSurface.h"
#include "DebugArrowUI.h"
#include "UIManager.h"

namespace
{
	const Vector3 MODEL_SCALE = Vector3(0.5f, 0.5f, 0.5f); // モデルのスケール

	constexpr float MAX_THROTLEINPUT = 1.0f; // 最大推力
	constexpr float CAPSELLE_RADIUS = 100.0f; // カプセルコライダーの半径
	constexpr float CAPSELLE_HEIGHT = 10.0f; // カプセルコライダーの高さ

	// 各操縦面の最大操舵角度（度数法）
	constexpr float MAX_LEFT_AILERON_ANGLE = 5.0 * 0.5f;
	constexpr float MAX_RIGHT_AILERON_ANGLE = 5.0 * 0.5f;
	constexpr float MAX_ELEVATOR_ANGLE = 3.0f * 0.5;//上下回転
	constexpr float MAX_RUDDER_ANGLE = 6.0;// 左右回転

	float DegToRad(float deg)
	{
		return deg * (3.1415f / 180.0f);
	}

}

Aircraft::Aircraft()
{

}
bool Aircraft::Start()
{
	return true;
}
void Aircraft::Init(const char* filePath, Vector3 initPos, float baseThrust)
{
	m_position = initPos;

	// モデルの初期化
	m_model.Init(filePath, nullptr, 0, enModelUpAxisZ, false);
	m_model.SetScale(MODEL_SCALE);
	m_model.Update();

	m_propeller.Init("Assets/modelData/Plane/Propeller.tkm", nullptr, 0, enModelUpAxisZ, false);
	m_propeller.SetScale(MODEL_SCALE);
	m_propeller.Update();


	// キャラクターコントローラーの初期化
	m_characterController.Init(CAPSELLE_RADIUS, CAPSELLE_HEIGHT, m_position);

	InitOrientation();

	InitWingPositionOffset();

	InitAllLiftingSurfaces();

	//エンジン
	m_engine = std::make_unique<Engine>();
	m_engine->SetBaseThrust(baseThrust);

	//デバッグ用のUIを登録
	for (int i = 0; i < DebugMomentArrowUIType::Count; i++)
	{
		auto debugMomentUI = std::make_unique<DebugArrowUI>();
		debugMomentUI->Init(collarType::enYerrow, 0.001);
		debugMomentUI->SetDisplayed(true);
		m_debugMomentUI[i] = debugMomentUI.get();

		auto debugForceUI = std::make_unique<DebugArrowUI>();
		debugForceUI->Init(collarType::enRed, 0.0005);
		debugForceUI->SetDisplayed(true);
		m_debugForceUI[i] = debugForceUI.get();

		auto debugMomentArmUI = std::make_unique<DebugArrowUI>();
		debugMomentArmUI->Init(collarType::enBlue, 2.0f);
		debugMomentArmUI->SetDisplayed(true);
		m_debugMomentArm[i] = debugMomentArmUI.get();

		UIManager::GetInstance().RegisterScreen("DebugMomentArrowUI" + std::to_string(i), std::move(debugMomentUI));
		UIManager::GetInstance().RegisterScreen("DebugForceArrowUI" + std::to_string(i), std::move(debugForceUI));
		UIManager::GetInstance().RegisterScreen("DebugMomentArmUI" + std::to_string(i), std::move(debugMomentArmUI));
	}


}

void Aircraft::Update()
{
	m_worldDirty = true;

	//翼の更新
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		//if (i == 3) {
			//翼の制御面の更新
		m_wings[i]->UpdateControlSurface();
		//翼の姿勢更新
		m_wings[i]->UpdateOrientation(m_state.orientation);
		/*}*/
	}

	//エンジンの更新
	{
		//エンジンの姿勢更新
		m_engine->UpdateOrientation(m_state.orientation);
		//推力ベクトルの更新
		m_engine->UpdateThrustForce();
	}

	//相対風の更新
	UpdateRelWind();

	//力を計算
	Vector3 force = ComputeForce();

	//加速度を計算
	m_accel = force / m_mass;

	//加速度を積分して速度を更新
	AddLinearVelocity(m_accel * g_gameTime->GetFrameDeltaTime());

	Vector3 debug = Vector3(0.0f, 0.0f, 0.0f);
	//移動実行
	m_position =
		m_characterController
		.AircraftExecute(
			m_linearVelocity/*debug*/,
			g_gameTime->GetFrameDeltaTime()
		);


	//モーメントの計算と姿勢の更新
	ComputeMoment();


	//モデルの更新
	UpdateModel();

	//デバッグ用のUIを更新
	for (int i = 0; i < DebugMomentArrowUIType::Count; i++)
	{

		//m_debugMomentUI[i]->UpdateTargetVec(m_wings[i]->ComputeMoment(m_state));
		//m_debugMomentUI[i]->UpdatePosition(m_position);

		//m_debugForceUI[i]->UpdateTargetVec(m_wings[i]->GetForce()+ m_engine->GetThrustForce());
		//m_debugForceUI[i]->UpdatePosition(m_position);

		//m_debugMomentArm[i]->UpdateTargetVec(m_wings[i]->GetWorldMomentArm());
		//m_debugMomentArm[i]->UpdatePosition(m_position);

	}



	////デバッグ用UIの更新
	//for (int i = 0; i < DebugMomentArrowUIType::Count; i++)
	//{

	//	m_debugMomentUI[i]->UpdateTargetVec(ComputeTotalMomentWorld());
	//	m_debugMomentUI[i]->UpdatePosition(m_position);

	//}

}
void Aircraft::Render(RenderContext& rc)
{
	m_model.Draw(rc);
	m_propeller.Draw(rc);
}
void Aircraft::SetControlInputs(float mainLeftInput, float mainRightInput, float tailInput, float verticalInput, bool isBoostOn, bool isThrottleCut)
{
	m_wings[static_cast<int>(WingType::MainLeft)]->SetControlInput(mainLeftInput);
	m_wings[static_cast<int>(WingType::MainRight)]->SetControlInput(mainRightInput);
	m_wings[static_cast<int>(WingType::Tail)]->SetControlInput(tailInput);
	m_wings[static_cast<int>(WingType::Vertical)]->SetControlInput(verticalInput);

	m_engine->SetBoostInput(isBoostOn);
	m_engine->SetThrottleCut(isThrottleCut);
}
void Aircraft::InitLiftingSurface(
	WingType wingsType,
	Quaternion orientation,
	Vector3 momentArm,
	float maxWingDeflectionAngle,
	bool isMirroed,
	bool isVertical
)
{
	m_wings[static_cast<int>(wingsType)] = new LiftingSurface(orientation, maxWingDeflectionAngle, momentArm, isMirroed, isVertical);
}


void Aircraft::InitOrientation()
{
	m_initWingsOrientation[static_cast<int>(WingType::MainLeft)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::MainRight)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::Tail)] = Quaternion::Identity;
	m_initWingsOrientation[static_cast<int>(WingType::Vertical)] = Quaternion::Identity;
}

void Aircraft::InitWingPositionOffset()
{
	m_wingPositionOffset[static_cast<int>(WingType::MainLeft)] = Vector3(3.5f, 0.0f, 0.0f);
	m_wingPositionOffset[static_cast<int>(WingType::MainRight)] = Vector3(-3.5f, 0.0f, 0.0f);
	m_wingPositionOffset[static_cast<int>(WingType::Tail)] = Vector3(0.0f, 0.0f, 3.00f);
	m_wingPositionOffset[static_cast<int>(WingType::Vertical)] = Vector3(0.0f, 0.0f, 3.00f);
}

void Aircraft::InitAllLiftingSurfaces()
{
	// 左右・上下の翼を順に初期化

	// 主翼（左）
	InitLiftingSurface(
		WingType::MainLeft,
		m_initWingsOrientation[static_cast<int>(WingType::MainLeft)],
		m_wingPositionOffset[static_cast<int>(WingType::MainLeft)],
		DegToRad(MAX_LEFT_AILERON_ANGLE),
		true
	);

	// 主翼（右）
	InitLiftingSurface(
		WingType::MainRight,
		m_initWingsOrientation[static_cast<int>(WingType::MainRight)],
		m_wingPositionOffset[static_cast<int>(WingType::MainRight)],
		DegToRad(MAX_RIGHT_AILERON_ANGLE)
	);

	// 水平尾翼
	InitLiftingSurface(
		WingType::Tail,
		m_initWingsOrientation[static_cast<int>(WingType::Tail)],
		m_wingPositionOffset[static_cast<int>(WingType::Tail)],
		DegToRad(MAX_ELEVATOR_ANGLE)
	);

	// 垂直尾翼
	InitLiftingSurface(
		WingType::Vertical,
		m_initWingsOrientation[static_cast<int>(WingType::Vertical)],
		m_wingPositionOffset[static_cast<int>(WingType::Vertical)],
		DegToRad(MAX_RUDDER_ANGLE),
		false,
		true
	);
}

Vector3 Aircraft::ComputeForce()
{
	Vector3 thrust = m_engine->GetThrustForce();

	Vector3 wingsForce = Vector3::Zero;
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		//if (i == 3) {

		m_wings[i]->ComputeForces(m_state);
		wingsForce += m_wings[i]->GetForce();

		//m_debugForceUI[i]->UpdateTargetVec(m_wings[i]->GetForce() /*+ m_engine->GetThrustForce()*/);

		//switch (static_cast<WingType>(i))
		//{
		//case WingType::MainLeft:
		//	m_debugForceUI[i]->UpdatePosition(m_position + Vector3(-200, 0, 0));
		//	break;

		//case WingType::MainRight:
		//	m_debugForceUI[i]->UpdatePosition(m_position + Vector3(200, 0, 0));

		//	break;

		//case WingType::Tail:
		//	m_debugForceUI[i]->UpdatePosition(m_position + Vector3(0, 0, -200));

		//	break;

		//case WingType::Vertical:
		//	m_debugForceUI[i]->UpdatePosition(m_position + Vector3(0, 0, 200));

		//	break;

		//default:
		//	break;
		//}

		//}
	}

	Vector3 force = thrust + wingsForce;
	force += ComputeGravity();


	//m_debugForceUI[0]->UpdateTargetVec(force /*+ m_engine->GetThrustForce()*/);
	//m_debugForceUI[0]->UpdatePosition(m_position);

	return force;
}

void Aircraft::ComputeMoment()
{
	//全ての翼のモーメント（world系）を合計
	Vector3 totalMomentWold = ComputeTotalMomentWorld();

	//モーメントの計算はオブジェクト座標系でしたいので
	// （オイラー方程式をそのまま使えるから）
	// 求めたモーメントをオブジェクト座標系に変換する。


	//ワールド座標の逆回転行列（オブジェクト行列）を求める。
	//移動は考慮しないので逆回転行列。
	//ワールド行列は直交行列なので転置行列で逆行列になる。
	Matrix objMat;
	objMat.MakeRotationFromQuaternion(m_state.orientation);
	objMat.Transpose();
	Vector3 totalMomentObj = totalMomentWold;
	objMat.Apply(totalMomentObj);

	//角加速度
	Vector3 angularAcc = ComputeOmegaDotBody(totalMomentObj);

	float delta = g_gameTime->GetFrameDeltaTime();



	//角速度
	//TODO:なぜか角速度を積分しない方が正しい動きになっている。
	m_angularVelocity = angularAcc * delta;

	Vector3 angularAxis = m_angularVelocity;
	if (m_angularVelocity.Length() > 0.00001) {
		angularAxis.Normalize();
	}

	K2_LOG("angularVelocity: %f, %f, %f\n", m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z);

	float angle = m_angularVelocity.Length() * g_gameTime->GetFrameDeltaTime();
	if (angle > 1e-5f) { // 小さすぎる場合は無視
		// 姿勢を更新
		Quaternion deltaQuaternion;
		deltaQuaternion.SetRotation(angularAxis, angle);
		m_state.orientation = m_state.orientation * deltaQuaternion;
		m_state.orientation.Normalize();
	}
}
void Aircraft::UpdateModel()
{
	//本体モデルの更新
	m_model.SetPosition(m_position);
	//m_characterController.SetPosition(m_position);
	m_model.SetRotation(m_state.orientation);
	m_model.Update();

	//プロペラモデルの更新
	float speed = m_linearVelocity.Length();

	m_propeller.SetPosition(m_position);
	Quaternion rot;
	m_propellerSpin.AddRotationDegZ(speed*0.03);
	rot.Multiply(m_propellerSpin, m_state.orientation);
	m_propeller.SetRotation(rot);
	m_propeller.Update();

}

Vector3 Aircraft::ComputeTotalMomentWorld()
{
	Vector3 totalMomentWorld = Vector3::Zero;
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		//if (i == 3) {
		if (m_wings[i]) {
			totalMomentWorld += m_wings[i]->ComputeMoment(m_state);
		}
		//}
	}
	return totalMomentWorld;
}

void Engine::UpdateThrustForce()
{
	float deltaTime = g_gameTime->GetFrameDeltaTime();
	float thrustScale = 0;//推力の大きさ

	//エンジンがかかっているときにスロットルを上げる。
	if (!m_isThrottleCut)
		m_throttleSmoothValue += deltaTime;
	else
		m_throttleSmoothValue -= deltaTime;

	if (m_throttleSmoothValue <= 0)m_throttleSmoothValue = 0;
	if (m_throttleSmoothValue >= MAX_THROTLEINPUT)m_throttleSmoothValue = MAX_THROTLEINPUT;

	if (m_isThrottleCut) {
		thrustScale = 0.0f;
		m_thrustForce = m_WoldeThrustDir * thrustScale;
		return;
	}

	m_throttleRatio = m_throttleSmoothValue / MAX_THROTLEINPUT;


	//入力値を非線形にして推力に反映。
	thrustScale = std::pow(m_throttleRatio, 2) * m_baseThrust;

	if (m_isBoostOn)
	{
		thrustScale *= m_boostMultiplier;
	}

	// 推力ベクトルを計算（機体の前方方向に推力をかける）
	m_thrustForce = m_WoldeThrustDir * thrustScale;
}
