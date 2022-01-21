#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include "Parameter.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

BOOL InitInput() {
	// �C���v�b�g8�̍쐬
	HRESULT hr = DirectInput8Create(instance_handle,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInputInterface,
		NULL);

	BOOL result_mouse;
	BOOL result_keyboard;
	if(FAILED(hr))
	{
		// ���s
		MessageBox(NULL, TEXT("�C���v�b�g�̏������Ɏ��s���܂���"), TEXT("���s"), MB_OK);
		return false;
	} else {
		// ����
		// �}�E�X�̍쐬
		result_mouse = InitMouse();
		result_keyboard = InitKeyboard();
	}

	// ���͏��̏�����
	ZeroMemory(&g_CurrentMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&g_PrevMouseState, sizeof(DIMOUSESTATE));

	return result_mouse & result_keyboard;
}

void ReleaseInput()
{
	// �f�o�C�X�̉��
	ReleaseMouse();
	ReleaseKeyboard();

	// �C���^�[�t�F�[�X�̉��
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
