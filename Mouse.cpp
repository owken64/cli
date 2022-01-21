#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include "Mouse.h"
#include "Parameter.h"
#include "Const.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define MOUSE_ON_VALUE (0x80)					//!< オン状態の値

//!< 協調レベルの設定
BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device);

//!< マウスの制御起動
BOOL StartMouseControl();


BOOL InitMouse() {

	g_MouseDevice = nullptr;

	// デバイス生成
	HRESULT hr = g_pInputInterface->CreateDevice(
		GUID_SysMouse,
		&g_MouseDevice,
		NULL);

	if (FAILED(hr))
	{
		return false;
	}

	// 入力フォーマットの指定
	hr = g_MouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	// 協調レベルの設定
	if (SetUpCooperativeLevel(g_MouseDevice) == false)
	{
		return false;
	}

	int count = 0;
	// 制御開始
	while (StartMouseControl() == false)
	{
		Sleep(100);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

	return true;

}

BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device)
{
	// 協調モードの設定
	if (FAILED(device->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)
	))
	{
		return false;
	}

	return true;
}

BOOL StartMouseControl()
{
	// デバイスが生成されてない
	if (g_MouseDevice == nullptr)
	{
		return false;
	}
	// 制御開始
	if (FAILED(g_MouseDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	g_MouseDevice->GetCapabilities(&cap);
	// ポーリング判定
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		/*
			PollはAcquireの前に行うとされていたが、
			Acquireの前で実行すると失敗したので
			後で実行するようにした
		*/
		if (FAILED(g_MouseDevice->Poll()))
		{
			return false;
		}
	}

	return true;
}

void ReleaseMouse() {
	if (g_MouseDevice != nullptr)
	{
		// 制御を停止
		g_MouseDevice->Unacquire();
		g_MouseDevice->Release();
		g_MouseDevice = nullptr;
	}
}

void UpdateMouse()
{
	if (g_MouseDevice == nullptr)
	{
		return;
	}

	// 更新前に最新マウス情報を保存する
	g_PrevMouseState = g_CurrentMouseState;
	prev_mouse_point = mouse_point;

	// マウスの状態を取得します
	HRESULT	hr = g_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	if (FAILED(hr))
	{
		g_MouseDevice->Acquire();
		hr = g_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	}

	POINT p;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&p);

	// スクリーン座標にクライアント座標に変換する
	ScreenToClient(FindWindow(WINDOW_CLASS_NAME, nullptr), &p);

	// 変換した座標を保存
	mouse_point.x = p.x;
	mouse_point.y = p.y;

	// 位置の変化量を計算
	mouse_vector.x = mouse_point.x - prev_mouse_point.x;
	mouse_vector.y = mouse_point.y - prev_mouse_point.y;
}

bool OnMouseDown(MouseButton button_type)
{
	if (!(g_PrevMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE) &&
		g_CurrentMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE)
	{
		return true;
	}

	return false;
}

bool OnMousePush(MouseButton button_type)
{
	if (g_PrevMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE &&
		g_CurrentMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE)
	{
		return true;
	}

	return false;
}

bool OnMouseUp(MouseButton button_type)
{
	if (g_PrevMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE &&
		!(g_CurrentMouseState.rgbButtons[button_type] & MOUSE_ON_VALUE))
	{
		return true;
	}

	return false;
}

bool MousePosInTheRect(RECT rect) {
	return mouse_point.x >= rect.left &&
		mouse_point.x < rect.right&&
		mouse_point.y >= rect.top &&
		mouse_point.y < rect.bottom;
}