#pragma once

#include <Windows.h>
#include <d3d9.h>
#include "Const.h"

struct Texture
{
	LPDIRECT3DTEXTURE9 TexutreData;	//!< テクスチャデータ
	int Width;						//!< 横幅
	int Height;						//!< 縦幅
};

/** @brief UV情報から矩形を作成するための構造体 */
struct UVRect
{
	/**
	* @brief Constructor@n
	* @param[in] pos_x テクスチャ座標X
	* @param[in] pos_y テクスチャ座標Y
	* @param[in] width 矩形の横幅
	* @param[in] height 矩形の縦幅
	* @param[in] tex_id 使用するテクスチャのID
	*/
	UVRect(float pos_x, float pos_y, float width, float height, TextureList tex_id) :
		m_TexturePosX(pos_x),
		m_TexturePosY(pos_y),
		m_RectWidth(width),
		m_RectHeight(height),
		m_TextureId(tex_id)
	{
	}

	float m_TexturePosX;		//!< テクスチャ座標X
	float m_TexturePosY;		//!< テクスチャ座標Y
	float m_RectWidth;			//!< 矩形横幅
	float m_RectHeight;			//!< 矩形縦幅
	TextureList m_TextureId;	//!< 対象のテクスチャID
};


struct UVAnimationRect : public UVRect
{
	/**
	* @brief Constructor@n
	* @param[in] pos_x テクスチャ座標X
	* @param[in] pos_y テクスチャ座標Y
	* @param[in] width 矩形の横幅
	* @param[in] height 矩形の縦幅
	* @param[in] tex_id 使用するテクスチャのID
	* @param[in] change_frame 切り替えにかかるフレーム
	* @param[in] next_id 次のアニメーションID
	*/
	UVAnimationRect(float pos_x, float pos_y, float width, float height, TextureList tex_id, float change_frame, float next_id) :
		UVRect(pos_x, pos_y, width, height, tex_id),
		m_ChangeFrame(change_frame),
		m_NextAnimationId(next_id)
	{}

	float m_ChangeFrame;		//!< 切り替えフレーム
	float m_NextAnimationId;	//!< 次のアニメーションID
};


struct FRECT {
	float left;
	float top;
	float right;
	float bottom;
};

/**
* @brief テクスチャデータ初期化関数@n
* ゲームで使用するテクスチャデータを保存出来るようにする
*/
void InitTexture();

/**
* @brief カテゴリー単位のテクスチャ解放関数@n
* 引数で指定されたカテゴリーのテクスチャを全て解放する
*/
void ReleaseCategoryTexture();

/**
* @brief テクスチャの全解放@n
* 読み込んでいるすべてのテクスチャを解放する@n
* ※この関数はEndEngineで使用されているので開発側で呼ぶ必要はない
*/
void AllReleaseTexture();

/**
* @brief テクスチャの読み込み@n
* 指定したされたパスのテクスチャを読み込み、カテゴリに登録する
* @return 読み込み結果(成功はtrue)
* @param[in] file_name 読み込むテクスチャ名(パス込み)
* @param[in] texture_id カテゴリー内のテクスチャID
*/
bool LoadTexture(const char* file_name, int texture_id);

/**
* @brief テクスチャデータの取得@n
* 指定されたカテゴリのテクスチャデータを取得する
* @return テクスチャデータ(取得失敗時はnullptr)
* @param[in] 取得したいテクスチャのID
*/
Texture* GetTexture(int texture_id);


/**
* @brief テクスチャ描画関数@n
* 指定された位置にテクスチャを描画する@n
* ※texture_dataはFile.hのGetTextureDataを使用してください。
* @param[in] x X軸描画座標
* @param[in] y Y軸描画座標
* @param[in] texture_data 描画で使用するテクスチャのデータ
*/
void DrawTexture(float x, float y, Texture* texture_data);

void DrawTexture(float x, float y, Texture* texture_data, FRECT tutv);

void DrawTexture(RECT rect, Texture* texture_data);

void DrawTexture(RECT rect, Texture* texture_data, FRECT tutv);

void DrawUVMappingTexture(float x, float y, UVRect* rect);

void DrawUVMappingTexture(float x, float y, Texture* texture_data, float texture_x, float texture_y, float sprite_width, float sprite_height);
