#pragma once

#include <Windows.h>
#include <d3d9.h>
#include "Const.h"

struct Texture
{
	LPDIRECT3DTEXTURE9 TexutreData;	//!< �e�N�X�`���f�[�^
	int Width;						//!< ����
	int Height;						//!< �c��
};

/** @brief UV��񂩂��`���쐬���邽�߂̍\���� */
struct UVRect
{
	/**
	* @brief Constructor@n
	* @param[in] pos_x �e�N�X�`�����WX
	* @param[in] pos_y �e�N�X�`�����WY
	* @param[in] width ��`�̉���
	* @param[in] height ��`�̏c��
	* @param[in] tex_id �g�p����e�N�X�`����ID
	*/
	UVRect(float pos_x, float pos_y, float width, float height, TextureList tex_id) :
		m_TexturePosX(pos_x),
		m_TexturePosY(pos_y),
		m_RectWidth(width),
		m_RectHeight(height),
		m_TextureId(tex_id)
	{
	}

	float m_TexturePosX;		//!< �e�N�X�`�����WX
	float m_TexturePosY;		//!< �e�N�X�`�����WY
	float m_RectWidth;			//!< ��`����
	float m_RectHeight;			//!< ��`�c��
	TextureList m_TextureId;	//!< �Ώۂ̃e�N�X�`��ID
};


struct UVAnimationRect : public UVRect
{
	/**
	* @brief Constructor@n
	* @param[in] pos_x �e�N�X�`�����WX
	* @param[in] pos_y �e�N�X�`�����WY
	* @param[in] width ��`�̉���
	* @param[in] height ��`�̏c��
	* @param[in] tex_id �g�p����e�N�X�`����ID
	* @param[in] change_frame �؂�ւ��ɂ�����t���[��
	* @param[in] next_id ���̃A�j���[�V����ID
	*/
	UVAnimationRect(float pos_x, float pos_y, float width, float height, TextureList tex_id, float change_frame, float next_id) :
		UVRect(pos_x, pos_y, width, height, tex_id),
		m_ChangeFrame(change_frame),
		m_NextAnimationId(next_id)
	{}

	float m_ChangeFrame;		//!< �؂�ւ��t���[��
	float m_NextAnimationId;	//!< ���̃A�j���[�V����ID
};


struct FRECT {
	float left;
	float top;
	float right;
	float bottom;
};

/**
* @brief �e�N�X�`���f�[�^�������֐�@n
* �Q�[���Ŏg�p����e�N�X�`���f�[�^��ۑ��o����悤�ɂ���
*/
void InitTexture();

/**
* @brief �J�e�S���[�P�ʂ̃e�N�X�`������֐�@n
* �����Ŏw�肳�ꂽ�J�e�S���[�̃e�N�X�`����S�ĉ������
*/
void ReleaseCategoryTexture();

/**
* @brief �e�N�X�`���̑S���@n
* �ǂݍ���ł��邷�ׂẴe�N�X�`�����������@n
* �����̊֐���EndEngine�Ŏg�p����Ă���̂ŊJ�����ŌĂԕK�v�͂Ȃ�
*/
void AllReleaseTexture();

/**
* @brief �e�N�X�`���̓ǂݍ���@n
* �w�肵�����ꂽ�p�X�̃e�N�X�`����ǂݍ��݁A�J�e�S���ɓo�^����
* @return �ǂݍ��݌���(������true)
* @param[in] file_name �ǂݍ��ރe�N�X�`����(�p�X����)
* @param[in] texture_id �J�e�S���[���̃e�N�X�`��ID
*/
bool LoadTexture(const char* file_name, int texture_id);

/**
* @brief �e�N�X�`���f�[�^�̎擾@n
* �w�肳�ꂽ�J�e�S���̃e�N�X�`���f�[�^���擾����
* @return �e�N�X�`���f�[�^(�擾���s����nullptr)
* @param[in] �擾�������e�N�X�`����ID
*/
Texture* GetTexture(int texture_id);


/**
* @brief �e�N�X�`���`��֐�@n
* �w�肳�ꂽ�ʒu�Ƀe�N�X�`����`�悷��@n
* ��texture_data��File.h��GetTextureData���g�p���Ă��������B
* @param[in] x X���`����W
* @param[in] y Y���`����W
* @param[in] texture_data �`��Ŏg�p����e�N�X�`���̃f�[�^
*/
void DrawTexture(float x, float y, Texture* texture_data);

void DrawTexture(float x, float y, Texture* texture_data, FRECT tutv);

void DrawTexture(RECT rect, Texture* texture_data);

void DrawTexture(RECT rect, Texture* texture_data, FRECT tutv);

void DrawUVMappingTexture(float x, float y, UVRect* rect);

void DrawUVMappingTexture(float x, float y, Texture* texture_data, float texture_x, float texture_y, float sprite_width, float sprite_height);
