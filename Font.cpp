#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include "Font.h"
#include "Parameter.h"
#include "Const.h"

// 静的ライブラリ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


bool InitFont()
{
	//フォントの生成 MSゴシック
	//デバイス,文字高さ,文字幅,フォントスタイル,ミップマップのレベル,斜体にするかどうか,文字セット,出力精度,出力品質,フォントピッチとファミリ,フォント名,フォントポインタ
	HRESULT hr = D3DXCreateFont(g_pD3DDevice, 10, 0, FW_NORMAL, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"ＭＳ ゴシック", &g_pFont);
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
