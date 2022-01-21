#include <dinput.h>
#include <dsound.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include "Parameter.h"
#include "Button.h"
#include "UI_Window.h"

// Window
HINSTANCE instance_handle;
LPDIRECTINPUT8 g_pInputInterface;
LPDIRECTINPUTDEVICE8 g_MouseDevice;
LPDIRECTINPUTDEVICE8 g_pKeyDevice;
LPDIRECT3D9 g_pD3DInterface;		// DirectGraphics�C���^�[�t�F�[�X
D3DPRESENT_PARAMETERS* g_pD3DPresentParam;
LPDIRECT3DDEVICE9 g_pD3DDevice;	// DirectGraphics�f�o�C�X
LPD3DXFONT    g_pFont; //�t�H���g

// Mouse
DIMOUSESTATE g_CurrentMouseState;		//!< �}�E�X�̌��݂̓��͏��
DIMOUSESTATE g_PrevMouseState;			//!< �}�E�X��1�t���[���O�̓��͏��
POINT mouse_point;						// �}�E�X�̈ʒu(�N���C�A���g���W)
POINT prev_mouse_point;                 // 1�t���[���O�̃}�E�X�̈ʒu(�N���C�A���g���W)
POINT mouse_vector;                     // 1�t���[���Ń}�E�X����������

// Keyboard
BYTE CurrentKeyState[256];              // �L�[�{�[�h�̌��݂̓��͏��
BYTE PrevKeyState[256];                 // �L�[�{�[�h��1�t���[���O�̓��͏��

// Sound
LPDIRECTSOUND8 g_SoundInterface;
LPDIRECTSOUNDBUFFER g_SoundBufferList[SoundFile::SoundFileMax];
SoundFile g_SoundBGM;                   // ����BGM�Ɏg���Ă���y��(�̔ԍ�)
SoundFile g_PrevSoundBGM;               // 1�t���[���O��BGM�Ɏg���Ă����y��(�̔ԍ�)
bool g_flgSEPlay[SoundFile::SoundFileMax]; // SE���Đ�������t���O

// Texture
Texture g_TextureList[TextureList::TextureMax];

// Button
Button g_ButtonList[ButtonList::ButtonMax];
bool g_ButtonVisible[ButtonList::ButtonMax]; // Visible

// Window(UI)
UI_Window g_UIWindowList[UIWindowList::UIWindowMax];
POINT g_UIWindowPoint[UIWindowList::UIWindowMax]; // Point(Position)
bool g_UIWindowVisible[UIWindowList::UIWindowMax]; // Visible 

// Player
POINT g_PointPlayer;
Direction g_DirectionPlayer;
float g_PlayerHungry;

// ������
bool g_TorigoyaVisible;