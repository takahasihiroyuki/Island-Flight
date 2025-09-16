#pragma once
#include "Light.h"


namespace nsK2EngineLow {
	class ModelRender
	{
	public:
		/// <summary>
		/// 通常描画用の初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		/// <param name="isShadowReciever">影を受ける側か</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationCrips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReciever = true
		);

		/// <summary>
		/// スカイキューブを作るときに使用。
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCubeModel(ModelInitData& initData);

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		void Draw(RenderContext& rc);

		/// <summary>
		/// ディファード用のモデルの描画
		/// </summary>
		/// <param name="rc"></param>
		void OnDraw(RenderContext& rc)
		{
			m_renderToGBufferModel.Draw(rc);
		}

		/// <summary>
		/// モデルを描画する(RenderingEngineで描画するときに呼び出す)。
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderModel(RenderContext& rc)
		{
			m_model.Draw(rc);
		}

		/// <summary>
		/// シャドウマップの描画をする（シャドウクラスで呼び出す）。
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="came"></param>
		void OnRenderShadowMap(RenderContext& rc, Camera& came);

		/// <summary>
		/// 行列を設定。
		/// </summary>
		/// <param name="mairix">行列。</param>
		void SetWorldMatrix(const Matrix& matrix);

		/// <summary>
		/// 座標、回転、拡大をすべて設定
		/// </summary>
		/// <param name="pos">座標の設定</param>
		/// <param name="rot">回転の設定</param>
		/// <param name="scale">拡大の設定</param>
		void SetTransform(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rot);
			SetScale(scale);
		}

		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="position">Vector3の座標</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// 座標を加算
		/// </summary>
		/// <param name="addPosition"></param>
		void AddPosition(Vector3 addPosition)
		{
			m_position += addPosition;
		}


		/// <summary>
		/// 回転の設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// 大きさの設定
		/// </summary>
		/// <param name="scale">大きさ</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// 大きさの設定
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetScale(float x, float y, float z)
		{
			SetScale({ x,y,z });
		};

		/// <summary>
		/// 大きさの設定(全ての軸同じ)
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(float scale)
		{
			SetScale({ scale,scale,scale });
		}

		/// <summary>
		/// 各種モデルのワールド行列を更新する。
		/// </summary>
		void UpdateWorldMatrixInModes();

		/// <summary>
		/// アニメーション再生。
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補完時間(単位：秒。)</param>
		/// <param name="animReset">アニメーションを最初からにするか。同じアニメーションから同じアニメーションに遷移するときに使う</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f, bool animReset = false)
		{
			m_animation.Play(animNo, interpolateTime, animReset);
		}

		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// アニメーション再生の速度を設定する。
		/// </summary>
		/// <param name="animationSpeed">数値の分だけ倍にする。</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

	private:

		/// <summary>
		/// スケルトンの初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// アニメーションの初期化。
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		/// <param name="enModelUpAxis">モデルの上向き。</param>
		void InitAnimation(
			AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);

		/// <summary>
		/// GBuffer描画モデルの初期化
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		/// <param name="isShadowReciever"></param>
		void InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever);

		/// <summary>
		/// 陰のモデルを初期化する。
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitShadowModel(const char* filePath, EnModelUpAxis enModelUpAxis);





	private:
		Skeleton					m_skeleton;									//スケルトン
		AnimationClip* m_animationClips = nullptr;					            //アニメーションクリップ。
		int							m_numAnimationClips = 0;					//アニメーションクリップの数。
		Animation					m_animation;								//アニメーション。
		float						m_animationSpeed = 1.0f;					//アニメーションの速度。

		Vector3						m_position = Vector3::Zero;				    //座標
		Vector3						m_scale = Vector3::One;					    //大きさ
		Quaternion					m_rotation = Quaternion::Identity;		    //回転

		Model						m_model;								    //モデルクラス
		Model						m_translucentModel;					        // 半透明モデル。
		ModelInitData				m_modelInitData;						    //モデルを初期化するための情報を設定するクラス


		bool						m_setWorldMatrix = false;			        // ワールド行列が外部から設定されている
		bool						m_isInit = false;					        //初期化したか

		SceneLight  			    m_sceneLight;                               //シーンライト

		Model                       m_renderToGBufferModel;	                    // RenderToGBufferで描画されるモデル
		Model                       m_shadowModel;							    //影描画用モデル

	};
}

