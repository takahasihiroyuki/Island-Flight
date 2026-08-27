#pragma once
#include "graphics/Lighting/Light.h"
#include "graphics/Core/GraphicsEnums.h"

namespace nsK2EngineLow {
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		/// <summary>
		/// 通常描画用の初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		/// <param name="isShadowReceiver">影を受ける側か</param>
		/// <param name="maxInstance">インスタンシング描画の最大数(1以下ならインスタンシング描画しない)</param>
		/// <param name="isFowardRender">フォワードレンダリングで描画するか</param>
		/// <param name="disableLayer">反射で映り込まないレイヤー</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationCrips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReceiver = true,
			size_t maxInstance = 1,
			bool isFowardRender = false,
			ReflectLayer disableLayer = ReflectLayer::enNone
		);

		/// <summary>
		/// 海描画用の初期化
		/// </summary>
		/// <param name="initData"></param>
		/// <param name="tkmFilePath">ファイルパス</param>
		void InitOcean(ModelInitData& initData, const char* tkmFilePath);

		/// <summary>
		/// スカイキューブを作るときに使用。
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCubeModel(ModelInitData& initData, const char* tkmFilePath);

		void InitSkyCubeReflectionModel(ModelInitData& initData);

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		void Draw(RenderContext& rc);

		void PlaneDraw();

		void OnDraw(RenderContext& rc)
		{
			if (!m_visible)return;
			if (m_isFowardRender) {
				m_frowardRenderModel.Draw(rc, m_maxInstance);
				return;
			}
			else {
				m_renderToGBufferModel.Draw(rc, m_maxInstance);

			}
		}

		void OnZPrepass(RenderContext& rc)
		{
			m_zprepassModel.Draw(rc, m_maxInstance);
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

		void OnRenderReflectionMap(RenderContext& rc, Camera& came);

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

		void AddRotation(const Quaternion& addRotation)
		{
			m_rotation.Add(addRotation);
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

		Matrix GetWorldMatrix() const
		{
			return m_renderToGBufferModel.GetWorldMatrix();
		}

		Model& GetModel()
		{
			if (m_isFowardRender) {
				return m_frowardRenderModel;
			}
			return m_renderToGBufferModel;
		}

		const Model& GetRenderToGBufferModel()const
		{
			return m_renderToGBufferModel;
		}
		Model& GetRenderToGBufferModel()
		{
			return m_renderToGBufferModel;
		}
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

		void ChangeAlbedoMap(const char* filePath, Texture& albedoMap)
		{
			m_model.ChangeAlbedoMap(filePath, albedoMap);
			m_renderToGBufferModel.ChangeAlbedoMap(filePath, albedoMap);
			m_shadowModel.ChangeAlbedoMap(filePath, albedoMap);
		}

		Vector3 GetPosition()const {
			return m_position;
		}

		void UpdateInstancingData(
			int instanceNo,
			const char* instanceName,
			const Vector3& pos,
			const Quaternion& rot,
			const Vector3& scale
		);

		Matrix GetWorldMatrixArray(size_t instanceNo)const
		{
			// インスタンス番号から行列のインデックスを取得する。
			int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];

			if (m_isEnableInstancingDraw)
			{
				return m_worldMatrixArray[matrixArrayIndex];
			}

			return Matrix::Identity;
		}

		void SetVisible(bool isVisible)
		{
			m_visible = isVisible;
		}


	private:
		/// <summary>
		/// インスタンスンシング描画用のデータを初期化。
		/// </summary>
		/// <param name="maxInstance"></param>
		void InitInstancingDraw(int maxInstance);

		/// <summary>
		/// スケルトンの初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void InitSkeleton(const char* filePath);

		void InitModelOnZprepass(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			bool isSkyCube = false
		);

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
		/// <param name="isShadowReceiver"></param>
		void InitModelOnRenderGBuffer(const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReceiver);

		/// <summary>
		/// 陰のモデルを初期化する。
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitShadowModel(const char* filePath, EnModelUpAxis enModelUpAxis);

		void InitReflectionModel(const char* filePath, EnModelUpAxis enModelUpAxis, ReflectLayer layer);

		//void InitSkyCubeReflectionModel(const char* filePath, EnModelUpAxis enModelUpAxis, ReflectLayer layer);

		void RemoveInstance(int instanceNo);

	private:
		Skeleton						m_skeleton;									//スケルトン
		AnimationClip* m_animationClips = nullptr;					            //アニメーションクリップ。
		int								m_numAnimationClips = 0;					//アニメーションクリップの数。
		Animation						m_animation;								//アニメーション。
		float							m_animationSpeed = 1.0f;					//アニメーションの速度。

		Vector3							m_position = Vector3::Zero;				    //座標
		Vector3							m_scale = Vector3::One;					    //大きさ
		Quaternion						m_rotation = Quaternion::Identity;		    //回転

		Model							m_model;								    //モデルクラス
		Model							m_frowardRenderModel;					    // フォワードレンダリングの描画パスで描画されるモデル
		ModelInitData					m_modelInitData;						    //モデルを初期化するための情報を設定するクラス


		bool							m_isFowardRender = false;					//フォワードレンダリングで描画するか
		bool							m_setWorldMatrix = false;			        // ワールド行列が外部から設定されている
		bool							m_isInit = false;					        //初期化したか
		std::map<ReflectLayer, bool>	m_enableReflection;   // 反射で映り込むかどうか
		SceneLight  					m_sceneLight;                               //シーンライト

		Model							m_renderToGBufferModel;	                    // RenderToGBufferで描画されるモデル
		Model							m_shadowModel;							    //影描画用モデル
		Model							m_zprepassModel;					// ZPrepassで描画されるモデル
		std::map<ReflectLayer, Model>	m_ReflectionModel;                          //反射マップ描画用モデル
		bool							m_isSkyCube = false;						// スカイキューブモデルかどうか
		std::unique_ptr<Matrix[]>		m_worldMatrixArray;							// ワールド行列の配列。
		EnModelUpAxis					m_modelUpAxis;								//モデルの上方向
		size_t							m_maxInstance = 0;							// インスタンスングの最大数
		bool							m_isEnableInstancingDraw = false;			// インスタンシング描画が有効か	
		StructuredBuffer				m_worldMatrixArraySB;						// ワールド行列の配列のストラクチャードバッファ。
		std::unique_ptr<int[]>			m_instanceNoToWorldMatrixArrayIndexTable;	// インスタンス番号からワールド行列の配列のインデックスに変換するテーブル。
		bool							m_visible = true;							// 表示・非表示
	};
}

