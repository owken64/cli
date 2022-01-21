#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Parameter.h"
#include "Graphics.h"
#include "Utility.h"
#include "Font.h"
#include "Texture.h"
#include "Button.h"
#include "UI_Window.h"

// 静的ライブラリ
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

void DrawRect(float x, float y, float width, float height, DWORD color)
{
	struct RectVertex
	{
		float X;
		float Y;
		float Z;
		float W;
		DWORD Color;
	};

	RectVertex verticies[4] =
	{
		{ x, y, 0.0f, 1.0f, color},
		{ x + width, y, 0.0f, 1.0f, color},
		{ x + width, y + height, 0.0f, 1.0f, color},
		{ x, y + height, 0.0f, 1.0f, color},
	};

	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_pD3DDevice->SetTexture(0, nullptr);

	g_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		verticies,
		sizeof(RectVertex)
	);
}

void DrawRect(RECT rect, DWORD color) {
	DrawRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, color);
}

void Draw()
{

	DrawTexture(0, 0, GetTexture(TextureList::Background));

	//DrawRect(100, 100, 100, 100, 0xffff000000);

	for (int i = 0; i < ButtonList::ButtonMax; i++) {
		if (g_ButtonVisible[i]){
		  DrawButton((ButtonList)i);
		}
	}

	for (int i = 0; i < UIWindowList::UIWindowMax; i++){
		if (g_UIWindowVisible[i]) {
			DrawUIWindow((UIWindowList)i);
		}
	}

	// 鳥小屋
	if (g_TorigoyaVisible) DrawTexture(200, 300, GetTexture(TextureList::Torigoya));

	if (g_DirectionPlayer == Direction::ToRight)	DrawTexture(g_PointPlayer.x, g_PointPlayer.y, GetTexture(TextureList::Reimu), { 0, 1.0 / 8.0, 1.0 / 12.0, 2.0 / 8.0 });
	else if (g_DirectionPlayer == Direction::ToLeft)	DrawTexture(g_PointPlayer.x, g_PointPlayer.y, GetTexture(TextureList::Reimu), { 0, 3.0 / 8.0, 1.0 / 12.0, 4.0 / 8.0 });

	DrawTexture(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 300, GetTexture(TextureList::Stand));

	DrawTexture(BALLOON_X,BALLOON_Y, GetTexture(TextureList::Balloon));

	DrawFont(L"博麗霊夢", RECT_BALLOON_NAME);
	char sample[] = "This is something too long sentence.If you see the returned sentence, then the development succeeds.";
	char* text_sample = formatedString(sample, 35);
	RECT rect_sample = { 505, 225, 700, 400 };
	WCHAR wtext_sample[256] = TEXT("");

	MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, text_sample, strlen(text_sample), wtext_sample, (sizeof wtext_sample) / 2);

	DrawFont(wtext_sample, rect_sample);

	ReleaseString(text_sample);

	if (g_PlayerHungry == 0.0f) DrawFont(L"Game Over", { 100,100,200, 125 });

	// テキストを描画
	RECT rect_x;
	RECT rect_y;
	rect_x = { 0, 0,  100, 25 };
	rect_y = { 0, 25, 100, 50 };

	// デバッグ描画用
	WCHAR w_text_x[256] = TEXT("");
	WCHAR w_text_y[256] = TEXT("");
	char* m_pos_x;
	char* m_pos_y;
	static char text_l_x[] = "Mouse Position X:";
	static char text_l_y[] = "Mouse Position Y:";
	char* text_x;
	char* text_y;

	m_pos_x = IntToString(mouse_point.x);
	m_pos_y = IntToString(mouse_point.y);
	text_x = concat(text_l_x, m_pos_x);
	text_y = concat(text_l_y, m_pos_y);
	#ifdef UNICODE
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, text_x, strlen(text_x), w_text_x, (sizeof w_text_x) / 2);
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, text_y, strlen(text_y), w_text_y, (sizeof w_text_y) / 2);			//もしくはmbstowcs(buf,str,(sizeof buf)/2);
	#else
		strcpy(buf, str);
	#endif
    DrawFont(w_text_x, rect_x);
	DrawFont(w_text_y, rect_y);

	ReleaseString(text_x);
	ReleaseString(text_y);
	ReleaseString(m_pos_x);
	ReleaseString(m_pos_y);
}
