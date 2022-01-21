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
LPDIRECT3D9 g_pD3DInterface;		// DirectGraphicsインターフェース
D3DPRESENT_PARAMETERS* g_pD3DPresentParam;
LPDIRECT3DDEVICE9 g_pD3DDevice;	// DirectGraphicsデバイス
LPD3DXFONT    g_pFont; //フォント

// Mouse
DIMOUSESTATE g_CurrentMouseState;		//!< マウスの現在の入力情報
DIMOUSESTATE g_PrevMouseState;			//!< マウスの1フレーム前の入力情報
POINT mouse_point;						// マウスの位置(クライアント座標)
POINT prev_mouse_point;                 // 1フレーム前のマウスの位置(クライアント座標)
POINT mouse_vector;                     // 1フレームでマウスが動いた量

// Keyboard
BYTE CurrentKeyState[256];              // キーボードの現在の入力情報
BYTE PrevKeyState[256];                 // キーボードの1フレーム前の入力情報

// Sound
LPDIRECTSOUND8 g_SoundInterface;
LPDIRECTSOUNDBUFFER g_SoundBufferList[SoundFile::SoundFileMax];
SoundFile g_SoundBGM;                   // 現在BGMに使われている楽曲(の番号)
SoundFile g_PrevSoundBGM;               // 1フレーム前にBGMに使われていた楽曲(の番号)
bool g_flgSEPlay[SoundFile::SoundFileMax]; // SEを再生させるフラグ

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

// 鳥小屋
bool g_TorigoyaVisible;