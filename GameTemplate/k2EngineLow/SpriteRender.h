#pragma once

namespace nsK2EngineLow {
	struct SpriteCB
	{
	};


	class SpriteRender
	{
	public:
		void Init(
			const char* filePath,
			const float width,
			const float height,
			AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans
		);

		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		void Update()
		{
			m_sprite.Update(
				m_position,
				m_rotation,
				m_scale,
				m_pivot
			);
		}

		void Draw(RenderContext& rc);
		void OnDraw(RenderContext& rc)
		{
			m_sprite.Draw(rc);
		}

	private:
		SpriteCB	m_spriteCB;
		Sprite		m_sprite;								//Spriteクラス
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;		//ピボット
		Vector3		m_position = Vector3::Zero;				//座標
		Vector3		m_scale = Vector3::One;					//大きさ
		Quaternion	m_rotation = Quaternion::Identity;	//回転
		bool		m_isInit = false;

	};
}

