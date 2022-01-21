#include <Windows.h>
#include "WindowProcedure.h"
#include "Const.h"
#include "Parameter.h"
#include "Input.h"
#include "Sound.h"
#include "Graphics.h"
#include "Draw.h"
#include "PlaySound.h"
#include "Procedure.h"

#pragma comment (lib, "winmm.lib")

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	instance_handle = hInstance;

	int width = WINDOW_WIDTH;
	int height = WINDOW_HEIGHT;

	// WNDCLASSEX�\���̂ɕK�v�ȏ���ݒ肷��
	WNDCLASSEX window_class =
	{
		sizeof(WNDCLASSEX),		// �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,	// �N���X�̃X�^�C��
		WindowProcedure,		// �E�B���h�E�v���V�[�W��
		0,				// �⏕������
		0,				// �⏕������
		hInstance,			// ���̃A�v���̃C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),// �A�C�R���摜
		LoadCursor(NULL, IDC_ARROW),	// �J�[�\���摜
		NULL,				// �w�i�u���V(�w�i�F)
		NULL,				// ���j���[��
		WINDOW_CLASS_NAME,		// �N���X��							
		NULL				// �������A�C�R��
	};

	// WNDCLASSEX�\���̂̓o�^
	if (RegisterClassEx(&window_class) == 0)
	{
		// ���s
		return 0;
	}


	HWND window_handle = CreateWindow(
		// �o�^���Ă���E�B���h�E�N���X�\���̖̂��O
		WINDOW_CLASS_NAME,
		// �E�B���h�E��(�^�C�g���o�[�ɕ\�������)
		TEXT("�E�B���h�E�����T���v��"),
		// �E�B���h�E�X�^�C��
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		// �\���ʒu
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// �T�C�Y
		width,
		height,
		// �e�n���h��
		NULL,
		// ���j���[�n���h��
		NULL,
		// �C���X�^���X�n���h��
		hInstance,
		// WM_CREATE���b�Z�[�W��lpparam�ɓn�������l
		NULL);

	if (window_handle == NULL)
	{
		return 0;
	}

	RECT window_rect;

	// �E�B���h�E�T�C�Y���擾
	if (GetWindowRect(window_handle, &window_rect) == false)
	{
		return 0;
	}

	RECT client_rect;

	// �E�B���h�E�T�C�Y���擾
	if (GetClientRect(window_handle, &client_rect) == false)
	{
		return 0;
	}

	// �t���[���T�C�Y�Z�o
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	// ���T�C�Y�p�T�C�Y�̎Z�o
	int resize_width = frame_size_x + width;
	int resize_height = frame_size_y + height;

	// �E�B���h�E�T�C�Y�X�V
	SetWindowPos(
		// �E�B���h�E�n���h��
		window_handle,
		// �z�u�����̃n���h��(NULL�ł悵)
		NULL,
		// �\�����WX
		CW_USEDEFAULT,
		// �\�����WY
		CW_USEDEFAULT,
		// ���T�C�Y����		
		resize_width,
		// ���T�C�Y�c��
		resize_height,
		// SWP_NOMOVE => �ʒu�ύX�Ȃ�
		SWP_NOMOVE);

	/*
	60FPS�ł�0.0016666�`�悾���ǁA
	1ms��萸�x�̍������Ԃ̎擾���ł��Ȃ��̂ŁA17ms�Ƃ���
*/
	DWORD frame_time = 17;

	// �@�D�v�����ԕۑ��p
	DWORD current = 0;	// ���ݎ��ԕۑ��p
	DWORD prev = 0;		// �ߋ����ԕۑ��p

	// �A�D���x��1ms�ɕύX
	timeBeginPeriod(1);

	// �B�D�ߋ����ԂƂ��ĕۑ�
	prev = timeGetTime();

	// ��������
	BOOL result = InitInput();
	if (result == false) {
		return 0;
	}

	result = InitSound();
	if (result == false) {
		MessageBox(NULL, TEXT("�T�E���h�̏������Ɏ��s���܂���"), TEXT("���s"), MB_OK);
		return 0;
	}

	result = InitDirectGraphics();
	if (result == false) {
		MessageBox(NULL, TEXT("�O���t�B�b�N�X�̏������Ɏ��s���܂���"), TEXT("���s"), MB_OK);
	}

	// �E�B���h�E��\����Ԃɂ���
	ShowWindow(window_handle, SW_SHOW);

	MSG msg;
	// ���C�����[�v
	while (true)
	{
		// �Q�[������
		UpdateInput();

		Procedure();

		bool ready = StartDraw();
		if (ready) {
			Draw();
		}
		EndDraw();

		PlaySound();

		// WM_QUIT�𒲂ׂă��[�v�𔲂���
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// �C�D���ݎ��ԂƂ��ĕۑ�
		current = timeGetTime();

		// �D�D�t���[�����Ԓ��ߔ���
		if (current - prev >= frame_time)
		{
			// �E�DSleep�őҋ@
			Sleep(1);

			// �F�D���Ԏ擾���Č��݂̎��Ԃɑ��
			current = timeGetTime();
		}

		// �G�D���݂̎��Ԃ��ߋ����Ԃɑ��
		prev = current;
	}

	// �H�DtimeEndPeriod�֐��Ő��x��߂�
	timeEndPeriod(1);

	// �I������
	ReleaseDirectGraphics();
	ReleaseSound();
	ReleaseInput();

	return 0;
}