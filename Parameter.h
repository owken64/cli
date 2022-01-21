#pragma once
#include <dinput.h>
#include <dsound.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include "Const.h"
#include "Texture.h"
#include "Button.h"
#include "UI_Window.h"

// Window
extern HINSTANCE instance_handle;
extern LPDIRECTINPUT8 g_pInputInterface;
extern LPDIRECTINPUTDEVICE8 g_MouseDevice;
extern LPDIRECTINPUTDEVICE8 g_pKeyDevice;
extern LPDIRECT3D9 g_pD3DInterface;		// DirectGraphics�C���^�[�t�F�[�X
extern D3DPRESENT_PARAMETERS* g_pD3DPresentParam;
extern LPDIRECT3DDEVICE9 g_pD3DDevice;	// DirectGraphics�f�o�C�X
extern LPD3DXFONT    g_pFont; //�t�H���g

// Mouse
extern DIMOUSESTATE g_CurrentMouseState;		//!< �}�E�X�̌��݂̓��͏��
extern DIMOUSESTATE g_PrevMouseState;			//!< �}�E�X��1�t���[���O�̓��͏��
extern POINT mouse_point;                       // �}�E�X�̈ʒu(�N���C�A���g���W)
extern POINT prev_mouse_point;                 // 1�t���[���O�̃}�E�X�̈ʒu(�N���C�A���g���W)
extern POINT mouse_vector;                     // 1�t���[���Ń}�E�X����������

// Keyboard
extern BYTE CurrentKeyState[256];              // �L�[�{�[�h�̌��݂̓��͏��
extern BYTE PrevKeyState[256];                 // �L�[�{�[�h��1�t���[���O�̓��͏��

// Sound
extern LPDIRECTSOUND8 g_SoundInterface;
extern LPDIRECTSOUNDBUFFER g_SoundBufferList[SoundFile::SoundFileMax];
extern SoundFile g_SoundBGM;                   // ����BGM�Ɏg���Ă���y��(�̔ԍ�)
extern SoundFile g_PrevSoundBGM;               // 1�t���[���O��BGM�Ɏg���Ă����y��(�̔ԍ�)
extern bool g_flgSEPlay[SoundFile::SoundFileMax]; // SE���Đ�������t���O

// Texture
extern Texture g_TextureList[TextureList::TextureMax];

// Button
extern Button buttonGameStart;
extern Button g_ButtonList[ButtonList::ButtonMax];
extern bool g_ButtonVisible[ButtonList::ButtonMax]; // Visible

// Window(UI)
extern UI_Window g_UIWindowList[UIWindowList::UIWindowMax];
extern POINT g_UIWindowPoint[UIWindowList::UIWindowMax]; // Point(Position)
extern bool g_UIWindowVisible[UIWindowList::UIWindowMax]; // Visible 

// Player
extern POINT g_PointPlayer;
extern Direction g_DirectionPlayer;
extern float g_PlayerHungry;

// ������
extern bool g_TorigoyaVisible;
