#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include "Parameter.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

BOOL InitInput() {
	// インプット8の作成
	HRESULT hr = DirectInput8Create(instance_handle,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputInterface,
		NULL);

	BOOL result_mouse;
	BOOL result_keyboard;
	if(FAILED(hr))
	{
		// 失敗
		MessageBox(NULL, TEXT("インプットの初期化に失敗しました"), TEXT("失敗"), MB_OK);
		return false;
	} else {
		// 成功
		// マウスの作成
		result_mouse = InitMouse();
		result_keyboard = InitKeyboard();
	}

	// 入力情報の初期化
	ZeroMemory(&g_CurrentMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&g_PrevMouseState, sizeof(DIMOUSESTATE));

	return result_mouse & result_keyboard;
}

void ReleaseInput()
{
	// デバイスの解放
	ReleaseMouse();
	ReleaseKeyboard();

	// インターフェースの解放
	if (g_pInputInterface != nullptr)
	{
		g_pInputInterface->Release();
		g_pInputInterface = nullptr;
	}
}

void UpdateInput()
{
	UpdateMouse();
	KeyUpdate();
}
