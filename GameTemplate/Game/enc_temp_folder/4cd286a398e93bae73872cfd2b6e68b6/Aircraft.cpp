#include "stdafx.h"
#include "Aircraft.h"
#include "LiftingSurface.h"
#include"EffectType.h"

namespace
{
	const Vector3 MODEL_SCALE = Vector3(0.5f, 0.5f, 0.5f);	// モデルのスケール

	constexpr float CAPSELLE_RADIUS = 100.0f;				// カプセルコライダーの半径
	constexpr float CAPSELLE_HEIGHT = 10.0f;				// カプセルコライダーの高さ

	// 各操縦面の最大操舵角度（度数法）
	constexpr float MAX_LEFT_AILERON_ANGLE = 1.0;
	constexpr float MAX_RIGHT_AILERON_ANGLE = 1.0;
	constexpr float MAX_ELEVATOR_ANGLE = 3.0f * 0.4;		//上下回転
	constexpr float MAX_RUDDER_ANGLE = 3.0 * 0.8;			// 左右回転

	constexpr float MAIN_WING_AREA = 1.0f;					//主翼の面積
	constexpr float TAIL_WING_AREA = 0.25f;					//水平尾翼の面積
	constexpr float VERTICAL_WING_AREA = 0.25f;				//垂直尾翼の面積

	//翼の位置オフセット
	const Vector3 MAIN_LEFT_WING_POS_OFFSET = Vector3(-3.5f, 0.0f, 0.0f);
	const Vector3 MAIN_RIGHT_WING_POS_OFFSET = Vector3(3.5f, 0.0f, 0.0f);
	const Vector3 TAIL_WING_POS_OFFSET = Vector3(0.0f, 0.0f, 2.0f);
	const Vector3 VERTICAL_WING_POS_OFFSET = Vector3(0.0f, 0.0f, 2.0f);


	/// <summary>
	/// 度数法をラジアンに変換
	/// </summary>
	/// <param name="deg"></param>
	/// <returns></returns>
	float DegToRad(float deg)
	{
		return deg * (3.1415f / 180.0f);
	}

}

Aircraft::Aircraft()
{
}
Aircraft::~Aircraft()
{
	DeleteGO(m_propellerSound);
	if (m_speedLineEffect) {
		m_speedLineEffect->Stop();
		DeleteGO(m_speedLineEffect);
	}
}
bool Aircraft::Start()
{
	// プロペラ音の初期化と再生
	{
		m_propellerSound = NewGO<SoundSource>(0);
		m_propellerSound->Init(static_cast<int>(SoundID::enPropellerSE), false);
		m_propellerSound->SetVolume(m_propellerSoundBaseVolume);
		m_propellerSound->Play(true);
	}
	// スピードラインエフェクトの初期化
	{
		EffectEngine::GetInstance()->ResistEffect(enSpeedLine, effectPath[enSpeedLine]);
	}
	return true;
}
void Aircraft::Init(const char* filePath, Vector3 initPos, float baseThrust)
{
	// モデルの初期化
	{
		m_model.Init(filePath, nullptr, 0, enModelUpAxisZ, false);
		m_model.SetScale(MODEL_SCALE);
		m_model.Update();
	}

	// 初期位置の設定
	m_state.position = initPos;

	// プロペラモデルの初期化
	{
		m_propeller.Init("Assets/modelData/Plane/Propeller.tkm", nullptr, 0, enModelUpAxisZ, false);
		m_propeller.SetScale(MODEL_SCALE);
		m_propeller.Update();
	}

	// キャラクターコントローラーの初期化
	m_characterController.Init(CAPSELLE_RADIUS, CAPSELLE_HEIGHT, m_state.position);

	//翼の初期化に使うオフセットを初期化
	{
		//翼の角度を初期化
		InitOrientation();
		//翼の位置を初期化
		InitWingPositionOffset();
	}

	//全ての翼の初期化
	InitAllLiftingSurfaces();

	//基礎推力を設定してエンジンを生成
	m_engine = std::make_unique<AircraftEngine>(baseThrust);

}

void Aircraft::Update()
{
	m_worldDirty = true;

	//翼の更新
	UpdateLiftingSurfaces();

	//エンジンの更新
	UpdateEngine();

	//力を計算
	Vector3 force = ComputeForce();

	//加速度を計算
	m_accel = force / m_mass;

	//加速度を積分して速度を更新
	AddLinearVelocity(m_accel * g_gameTime->GetFrameDeltaTime());

	MoveAndResolveCollision();


	//モーメントの計算と姿勢の更新
	UpdateAngularMotion();


	UpdatePresentation();

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
	float area,
	bool isMirroed,
	bool isVertical
)
{
	m_wings[static_cast<int>(wingsType)] = std::make_unique<LiftingSurface>(orientation, maxWingDeflectionAngle, momentArm, area, isMirroed, isVertical);
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
	m_wingPositionOffset[static_cast<int>(WingType::MainLeft)] = Vector3(-3.5f, 0.0f, 0.0f);
	m_wingPositionOffset[static_cast<int>(WingType::MainRight)] = Vector3(3.5f, 0.0f, 0.0f);
	m_wingPositionOffset[static_cast<int>(WingType::Tail)] = Vector3(0.0f, 0.0f, 2.0f);
	m_wingPositionOffset[static_cast<int>(WingType::Vertical)] = Vector3(0.0f, 0.0f, 2.0f);
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
		MAIN_WING_AREA,
		true
	);

	// 主翼（右）
	InitLiftingSurface(
		WingType::MainRight,
		m_initWingsOrientation[static_cast<int>(WingType::MainRight)],
		m_wingPositionOffset[static_cast<int>(WingType::MainRight)],
		DegToRad(MAX_RIGHT_AILERON_ANGLE),
		MAIN_WING_AREA
	);

	// 水平尾翼
	InitLiftingSurface(
		WingType::Tail,
		m_initWingsOrientation[static_cast<int>(WingType::Tail)],
		m_wingPositionOffset[static_cast<int>(WingType::Tail)],
		DegToRad(MAX_ELEVATOR_ANGLE),
		TAIL_WING_AREA
	);

	// 垂直尾翼
	InitLiftingSurface(
		WingType::Vertical,
		m_initWingsOrientation[static_cast<int>(WingType::Vertical)],
		m_wingPositionOffset[static_cast<int>(WingType::Vertical)],
		DegToRad(MAX_RUDDER_ANGLE),
		VERTICAL_WING_AREA,
		false,
		true
	);
}

void Aircraft::UpdateLiftingSurfaces()
{
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		//翼の制御面の更新
		m_wings[i]->UpdateControlSurface();
		//翼の姿勢更新
		m_wings[i]->UpdateOrientation(m_state.orientation);
	}
}
void Aircraft::UpdateEngine()
{
	//エンジンの姿勢更新
	m_engine->UpdateOrientation(m_state.orientation);
	//推力ベクトルの更新
	m_engine->UpdateThrustForce();
}
void Aircraft::MoveAndResolveCollision()
{
	SweepHit sweepHit = SweepHit();

	//トランスレーションロックがonなら移動させない
	if (m_lockPosition) {

		Vector3 debug = Vector3(0.0f, 0.0f, 0.0f);

		//移動実行
		m_state.position =
			m_characterController
			.AircraftExecute(
				debug,
				g_gameTime->GetFrameDeltaTime(),
				&sweepHit
			);

	}
	else {
		//移動実行
		m_state.position =
			m_characterController
			.AircraftExecute(
				m_state.linearVelocity,
				g_gameTime->GetFrameDeltaTime(),
				&sweepHit
			);
	}

	//衝突処理
	if (sweepHit.hit) {
		OnCollisionHit(sweepHit);
	}
}

void Aircraft::UpdatePresentation()
{
	//モデルの更新
	UpdateModel();

	//プロペラ音の更新
	UpdatePropellerSound();

	PlayEffects();

}


Vector3 Aircraft::ComputeForce()
{
	Vector3 thrust = m_engine->GetThrustForce();

	Vector3 wingsForce = Vector3::Zero;
	for (int i = 0; i < static_cast<int>(WingType::Count); i++)
	{
		m_wings[i]->ComputeForces(m_state);
		wingsForce += m_wings[i]->GetForce();
	}
	Vector3 force = thrust + wingsForce;
	force += ComputeGravity();

	return force;
}

void Aircraft::UpdateAngularMotion()
{
	if (m_lockRotation)return;
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
	m_state.angularVelocity += angularAcc * delta;
	// 回転ダンピング
	m_state.angularVelocity *= 0.5f;

	Vector3 angularAxis = m_state.angularVelocity;
	if (m_state.angularVelocity.Length() > 0.00001) {
		angularAxis.Normalize();
	}

	K2_LOG("angularVelocity: %f, %f, %f\n", m_state.angularVelocity.x, m_state.angularVelocity.y, m_state.angularVelocity.z);

	float angle = m_state.angularVelocity.Length() * g_gameTime->GetFrameDeltaTime();
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
	m_model.SetPosition(m_state.position);
	//m_characterController.SetPosition(m_position);
	m_model.SetRotation(m_state.orientation);
	m_model.Update();

	//プロペラモデルの更新
	float speed = m_state.linearVelocity.Length();
	m_propeller.SetPosition(m_state.position);
	Quaternion rot;
	m_propellerSpin.AddRotationDegZ(speed * 0.03);
	rot.Multiply(m_propellerSpin, m_state.orientation);
	m_propeller.SetRotation(rot);
	m_propeller.Update();

}

void Aircraft::UpdatePropellerSound()
{
	if (m_propellerSound) {
		//プロペラ音の音量更新
		float thrustRatio = m_engine->GetThrustScale() / m_engine->GetBaseThrust();// 基本推力に対する現在の推力の割合
		m_propellerSound->SetVolume(m_propellerSoundBaseVolume * thrustRatio);
	}
}

void Aircraft::PlayEffects()
{

	if (m_engine->GetIsBoostOn()) {//ブースと状態なら
		if (!m_speedLineEffect) {
			m_speedLineEffect = NewGO<EffectEmitter>(0);
			m_speedLineEffect->Init(enSpeedLine);
		}
		m_speedLineEffect->SetPosition(g_camera3D->GetPosition());
		m_speedLineEffect->SetScale(Vector3::One * 1);
		Quaternion CameraRotation = Quaternion::Identity;
		Matrix RotationMat;
		RotationMat = g_camera3D->GetCameraRotation();//カメラの回転行列を取得
		CameraRotation.SetRotation(RotationMat);//行列からクォータニオンに変換
		m_speedLineEffect->SetRotation(CameraRotation);
		if (!m_speedLineEffect->IsPlay()) {//再生中じゃなければ再生する。
			m_speedLineEffect->Play();
		}
	}
	else {
		if (!m_speedLineEffect)return;
		DeleteGO(m_speedLineEffect);
		m_speedLineEffect = nullptr;
	}
}

Vector3 Aircraft::ComputeTotalMomentWorld()
{
	Vector3 totalMomentWorld = Vector3::Zero;
	for (int i = 0; i < static_cast<int>(WingType::Count); i++) {
		if (m_wings[i]) {
			totalMomentWorld += m_wings[i]->ComputeMoment();
		}
	}

	return totalMomentWorld;
}

void Aircraft::OnCollisionHit(const SweepHit& sweepHit)
{
	//跳ね返り計算
	CalculateBounce(sweepHit);
}

void Aircraft::CalculateBounce(const SweepHit& sweepHit)
{
	//ヒットしたオブジェクトの法線
	Vector3 HitObjectNormal = sweepHit.normal;
	if (HitObjectNormal.LengthSq() < 1e-8f) return;
	HitObjectNormal.Normalize();
	//法線方向の速度成分
	float normalVelocity = m_state.linearVelocity.Dot(HitObjectNormal);

	//法線方向に向かって移動していなければ跳ね返り計算しない
	if (normalVelocity > 0.0f) return;

	const float restitution = 0.3f;//反発係数(反発したときにどのくらい速度が減るか)
	m_state.linearVelocity -= HitObjectNormal * ((1.0f + restitution) * normalVelocity);
}

Vector3 Aircraft::GetWingMomentWorld(WingType wingType) const
{

	size_t index = static_cast<size_t>(wingType);
	return m_wings[index]->GetWorldMoment();

}

Vector3 Aircraft::GetWingForceWorld(WingType wingType) const
{
	size_t index = static_cast<size_t>(wingType);
	return m_wings[index]->GetForce();
}

Vector3 Aircraft::GetWingMomentArmWorld(WingType wingType) const
{
	size_t index = static_cast<size_t>(wingType);
	return m_wings[index]->GetWorldMomentArm();
}

