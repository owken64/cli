#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include "Font.h"
#include "Parameter.h"
#include "Const.h"

// �ÓI���C�u����
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


bool InitFont()
{
	//�t�H���g�̐��� MS�S�V�b�N
	//�f�o�C�X,��������,������,�t�H���g�X�^�C��,�~�b�v�}�b�v�̃��x��,�Α̂ɂ��邩�ǂ���,�����Z�b�g,�o�͐��x,�o�͕i��,�t�H���g�s�b�`�ƃt�@�~��,�t�H���g��,�t�H���g�|�C���^
	HRESULT hr = D3DXCreateFont(g_pD3DDevice, 10, 0, FW_NORMAL, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"�l�r �S�V�b�N", &g_pFont);
	if FAILED(hr) { return(false); }
	return true;
}

void ReleaseFont() {
	g_pFont->Release();
}

void DrawFont(LPCWSTR text, RECT rect) {
	//g_pFont->DrawText(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
	g_pFont->DrawText(NULL, text, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(255, 0, 0, 0));
}
