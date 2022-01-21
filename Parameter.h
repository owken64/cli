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
extern LPDIRECT3D9 g_pD3DInterface;		// DirectGraphicsインターフェース
extern D3DPRESENT_PARAMETERS* g_pD3DPresentParam;
extern LPDIRECT3DDEVICE9 g_pD3DDevice;	// DirectGraphicsデバイス
extern LPD3DXFONT    g_pFont; //フォント

// Mouse
extern DIMOUSESTATE g_CurrentMouseState;		//!< マウスの現在の入力情報
extern DIMOUSESTATE g_PrevMouseState;			//!< マウスの1フレーム前の入力情報
extern POINT mouse_point;                       // マウスの位置(クライアント座標)
extern POINT prev_mouse_point;                 // 1フレーム前のマウスの位置(クライアント座標)
extern POINT mouse_vector;                     // 1フレームでマウスが動いた量

// Keyboard
extern BYTE CurrentKeyState[256];              // キーボードの現在の入力情報
extern BYTE PrevKeyState[256];                 // キーボードの1フレーム前の入力情報

// Sound
extern LPDIRECTSOUND8 g_SoundInterface;
extern LPDIRECTSOUNDBUFFER g_SoundBufferList[SoundFile::SoundFileMax];
extern SoundFile g_SoundBGM;                   // 現在BGMに使われている楽曲(の番号)
extern SoundFile g_PrevSoundBGM;               // 1フレーム前にBGMに使われていた楽曲(の番号)
extern bool g_flgSEPlay[SoundFile::SoundFileMax]; // SEを再生させるフラグ

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

// 鳥小屋
extern bool g_TorigoyaVisible;
