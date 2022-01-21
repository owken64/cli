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

	// WNDCLASSEX構造体に必要な情報を設定する
	WNDCLASSEX window_class =
	{
		sizeof(WNDCLASSEX),		// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,	// クラスのスタイル
		WindowProcedure,		// ウィンドウプロシージャ
		0,				// 補助メモリ
		0,				// 補助メモリ
		hInstance,			// このアプリのインスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),// アイコン画像
		LoadCursor(NULL, IDC_ARROW),	// カーソル画像
		NULL,				// 背景ブラシ(背景色)
		NULL,				// メニュー名
		WINDOW_CLASS_NAME,		// クラス名							
		NULL				// 小さいアイコン
	};

	// WNDCLASSEX構造体の登録
	if (RegisterClassEx(&window_class) == 0)
	{
		// 失敗
		return 0;
	}


	HWND window_handle = CreateWindow(
		// 登録しているウィンドウクラス構造体の名前
		WINDOW_CLASS_NAME,
		// ウィンドウ名(タイトルバーに表示される)
		TEXT("ウィンドウ生成サンプル"),
		// ウィンドウスタイル
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		// 表示位置
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// サイズ
		width,
		height,
		// 親ハンドル
		NULL,
		// メニューハンドル
		NULL,
		// インスタンスハンドル
		hInstance,
		// WM_CREATEメッセージでlpparamに渡したい値
		NULL);

	if (window_handle == NULL)
	{
		return 0;
	}

	RECT window_rect;

	// ウィンドウサイズを取得
	if (GetWindowRect(window_handle, &window_rect) == false)
	{
		return 0;
	}

	RECT client_rect;

	// ウィンドウサイズを取得
	if (GetClientRect(window_handle, &client_rect) == false)
	{
		return 0;
	}

	// フレームサイズ算出
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	// リサイズ用サイズの算出
	int resize_width = frame_size_x + width;
	int resize_height = frame_size_y + height;

	// ウィンドウサイズ更新
	SetWindowPos(
		// ウィンドウハンドル
		window_handle,
		// 配置順序のハンドル(NULLでよし)
		NULL,
		// 表示座標X
		CW_USEDEFAULT,
		// 表示座標Y
		CW_USEDEFAULT,
		// リサイズ横幅		
		resize_width,
		// リサイズ縦幅
		resize_height,
		// SWP_NOMOVE => 位置変更なし
		SWP_NOMOVE);

	/*
	60FPSでは0.0016666描画だけど、
	1msより精度の高い時間の取得ができないので、17msとする
*/
	DWORD frame_time = 17;

	// ①．計測時間保存用
	DWORD current = 0;	// 現在時間保存用
	DWORD prev = 0;		// 過去時間保存用

	// ②．精度を1msに変更
	timeBeginPeriod(1);

	// ③．過去時間として保存
	prev = timeGetTime();

	// 初期処理
	BOOL result = InitInput();
	if (result == false) {
		return 0;
	}

	result = InitSound();
	if (result == false) {
		MessageBox(NULL, TEXT("サウンドの初期化に失敗しました"), TEXT("失敗"), MB_OK);
		return 0;
	}

	result = InitDirectGraphics();
	if (result == false) {
		MessageBox(NULL, TEXT("グラフィックスの初期化に失敗しました"), TEXT("失敗"), MB_OK);
	}

	// ウィンドウを表示状態にする
	ShowWindow(window_handle, SW_SHOW);

	MSG msg;
	// メインループ
	while (true)
	{
		// ゲーム処理
		UpdateInput();

		Procedure();

		bool ready = StartDraw();
		if (ready) {
			Draw();
		}
		EndDraw();

		PlaySound();

		// WM_QUITを調べてループを抜ける
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

		// ④．現在時間として保存
		current = timeGetTime();

		// ⑤．フレーム時間超過判定
		if (current - prev >= frame_time)
		{
			// ⑥．Sleepで待機
			Sleep(1);

			// ⑦．時間取得して現在の時間に代入
			current = timeGetTime();
		}

		// ⑧．現在の時間を過去時間に代入
		prev = current;
	}

	// ⑨．timeEndPeriod関数で精度を戻す
	timeEndPeriod(1);

	// 終了処理
	ReleaseDirectGraphics();
	ReleaseSound();
	ReleaseInput();

	return 0;
}