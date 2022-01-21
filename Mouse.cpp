#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>
#include "Mouse.h"
#include "Parameter.h"
#include "Const.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define MOUSE_ON_VALUE (0x80)					//!< �I����Ԃ̒l

//!< �������x���̐ݒ�
BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device);

//!< �}�E�X�̐���N��
BOOL StartMouseControl();


BOOL InitMouse() {

	g_MouseDevice = nullptr;

	// �f�o�C�X����
	HRESULT hr = g_pInputInterface->CreateDevice(
		GUID_SysMouse,
		&g_MouseDevice,
		NULL);

	if (FAILED(hr))
	{
		return false;
	}

	// ���̓t�H�[�}�b�g�̎w��
	hr = g_MouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	// �������x���̐ݒ�
	if (SetUpCooperativeLevel(g_MouseDevice) == false)
	{
		return false;
	}

	int count = 0;
	// ����J�n
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
	// �������[�h�̐ݒ�
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
	// �f�o�C�X����������ĂȂ�
	if (g_MouseDevice == nullptr)
	{
		return false;
	}
	// ����J�n
	if (FAILED(g_MouseDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	g_MouseDevice->GetCapabilities(&cap);
	// �|�[�����O����
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// �|�[�����O�J�n
		/*
			Poll��Acquire�̑O�ɍs���Ƃ���Ă������A
			Acquire�̑O�Ŏ��s����Ǝ��s�����̂�
			��Ŏ��s����悤�ɂ���
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
		// ������~
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

	// �X�V�O�ɍŐV�}�E�X����ۑ�����
	g_PrevMouseState = g_CurrentMouseState;
	prev_mouse_point = mouse_point;

	// �}�E�X�̏�Ԃ��擾���܂�
	HRESULT	hr = g_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	if (FAILED(hr))
	{
		g_MouseDevice->Acquire();
		hr = g_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState);
	}

	POINT p;
	// �}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&p);

	// �X�N���[�����W�ɃN���C�A���g���W�ɕϊ�����
	ScreenToClient(FindWindow(WINDOW_CLASS_NAME, nullptr), &p);

	// �ϊ��������W��ۑ�
	mouse_point.x = p.x;
	mouse_point.y = p.y;

	// �ʒu�̕ω��ʂ��v�Z
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