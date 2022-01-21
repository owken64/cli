#include "Keyboard.h"
#include "Parameter.h"
#include "Const.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

BOOL InitKeyboard()
{
	HRESULT hr;

	// IDirectInputDevice8�C���^�[�t�F�C�X�̎擾
	hr = g_pInputInterface->CreateDevice(GUID_SysKeyboard, &g_pKeyDevice, NULL);
	if (FAILED(hr))
	{
		MessageBox(FindWindow(WINDOW_CLASS_NAME, nullptr), TEXT("error"), TEXT("�G���[B"), MB_OK);
		return false;
	}

	// �f�o�C�X�̃t�H�[�}�b�g�̐ݒ�
	hr = g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(FindWindow(WINDOW_CLASS_NAME, nullptr), TEXT("error"), TEXT("�G���[C"), MB_OK);
		return false;
	}

	// �������[�h�̐ݒ�
	hr = g_pKeyDevice->SetCooperativeLevel(FindWindow(WINDOW_CLASS_NAME, nullptr), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(FindWindow(WINDOW_CLASS_NAME, nullptr), TEXT("error"), TEXT("�G���[D"), MB_OK);
		return false;
	}

	// �f�o�C�X�̎擾�J�n
	g_pKeyDevice->Acquire();

	return true;
}

void ReleaseKeyboard()
{
	g_pKeyDevice->Unacquire();
	g_pKeyDevice->Release();
}

void KeyUpdate()
{
	HRESULT hr;
	memcpy(PrevKeyState, CurrentKeyState, sizeof(CurrentKeyState));

	// �L�[�{�[�h�f�o�C�X�̃Q�b�^�[
	hr = g_pKeyDevice->GetDeviceState(256, CurrentKeyState);
	if (SUCCEEDED(hr))
	{

	}
	else if (hr == DIERR_INPUTLOST) {
		g_pKeyDevice->Acquire();
	}
}

bool OnKeyboardDown(int keycode) {
	if (!(PrevKeyState[keycode] & 0x80) && (CurrentKeyState[keycode] & 0x80))
	{
		return true;
	}
	else return false;
}

bool OnKeyboardPush(int keycode) {
	if ((PrevKeyState[keycode] & 0x80) && (CurrentKeyState[keycode] & 0x80)) {
		return true;
	}
	else return false;
}

bool OnKeyboardUp(int keycode) {
	if ((PrevKeyState[keycode] & 0x80) && !(CurrentKeyState[keycode] & 0x80))
	{
		return true;
	}
	else return false;
}
