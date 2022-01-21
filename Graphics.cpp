#include <d3d9.h>
#include <d3dx9.h>
#include "Parameter.h"
#include "Const.h"
#include "Font.h"
#include "Texture.h"

// 静的ライブラリ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

bool InitDirectGraphics()
{
	// インターフェース作成
	g_pD3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3DInterface == NULL)
	{
		// 作成失敗
		return false;
	}

	g_pD3DPresentParam = new D3DPRESENT_PARAMETERS;
	if (g_pD3DPresentParam == NULL)
	{
		return false;
	}
	ZeroMemory(g_pD3DPresentParam, sizeof(D3DPRESENT_PARAMETERS));

	// バックバッファの数 => 1
	g_pD3DPresentParam->BackBufferCount = 1;
	// バックバッファのフォーマット => D3DFMT_UNKNOWN(フォーマットを知りません)
	g_pD3DPresentParam->BackBufferFormat = D3DFMT_UNKNOWN;
	// ウィンドウモード設定 => 定数で切り替え
	g_pD3DPresentParam->Windowed = true;

	// スワップエフェクト設定 => ディスプレイドライバ依存
	// スワップエフェクト => バックバッファとフロントバッファへの切り替え方法
	g_pD3DPresentParam->SwapEffect = D3DSWAPEFFECT_DISCARD;

	// DirectGraphicsDeviceの作成
	if (FAILED(g_pD3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
	    FindWindow(WINDOW_CLASS_NAME,nullptr),
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		g_pD3DPresentParam,
		&g_pD3DDevice)))
	{
		return false;
	}

	// ビューポートパラメータ
	D3DVIEWPORT9 view_port;

	// ビューポートの左上座標
	view_port.X = 0;
	view_port.Y = 0;
	// ビューポートの幅
	view_port.Width = g_pD3DPresentParam->BackBufferWidth;
	// ビューポートの高さ
	view_port.Height = g_pD3DPresentParam->BackBufferHeight;
	// ビューポート深度設定
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// ビューポート設定
	if (FAILED(g_pD3DDevice->SetViewport(&view_port)))
	{
		return false;
	}

	bool result = InitFont();
	if (result == false) {
		MessageBox(nullptr, TEXT("フォントの初期化に失敗しました"), TEXT("失敗"), MB_OK);
		return false;
	}

	InitTexture();

	return true;
}

void ReleaseDirectGraphics()
{
	AllReleaseTexture();
	ReleaseFont();
	g_pD3DDevice->Release();
	g_pD3DInterface->Release();
}

bool StartDraw()
{
	// １．シーンクリア
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0);

	// ２．シーン描画開始
	if (D3D_OK == g_pD3DDevice->BeginScene())
	{
		return true;
	}

	return false;
}

void EndDraw()
{
	// ３．シーン描画終了
	g_pD3DDevice->EndScene();
	// ４．バッファ転送
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
