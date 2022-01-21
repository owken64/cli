#include "WindowProcedure.h"
#include <Windows.h>
#include "Parameter.h"
#include "Utility.h"
#include "Sound.h"
#include "Draw.h"
#include "Texture.h"
#include "Button.h"
#include "Procedure.h"

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	//static TCHAR szText[] = TEXT(text);
	HDC hdc;
	PAINTSTRUCT ps;

	// ���b�Z�[�W�ɑ΂���Ή�����
	switch (msg)
	{
	case WM_SHOWWINDOW:
		InitParameter();

		break;

		// �E�B���h�E�̕���A�C�R�����N���b�N���ꂽ
	case WM_CLOSE:

		StopSoundFile(SoundFile::TestBGM);
		PostQuitMessage(0);
		break;
		// �Ή����Ȃ�
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		// PlaySoundFile(SoundFile::Click, false);
		break;
	case WM_KEYDOWN:
		/*
		hdc = GetDC(hwnd);
		if (CurrentKeyState[DIK_RETURN] & 0x80) {
			TextOut(hdc, 0, 50, TEXT("RETURN"), lstrlen(TEXT("RETURN")));
		}
		ReleaseDC(hwnd, hdc);
		*/
		break;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
		break;
	}

	// �Ή���������0��Ԃ�
	return 0;
}
