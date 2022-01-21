#include "Procedure.h"
#include "Const.h"
#include "Parameter.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Sound.h"
#include "Texture.h"
#include "UI_Window.h"
#include <Windows.h>

void InitParameter() {
	// Sound
	static char filename[] = "Thank_you_for_playing.wav";
	static char filename_se[] = "CursorMove.wav";

	LoadSoundFile(SoundFile::TestBGM, filename);
	LoadSoundFile(SoundFile::TestBGM2, "NightRain.wav");
	LoadSoundFile(SoundFile::Click, filename_se);
	LoadSoundFile(SoundFile::ButtonPush, "button_push.wav");

	g_SoundBGM = SoundFile::TestBGM;
	g_PrevSoundBGM = SoundFile::TestBGM;

	PlaySoundFile(SoundFile::TestBGM, true);
	LoadTexture("Marisa3.png", TextureList::Background);
	LoadTexture("button.png", TextureList::ButtonGameStart);
	LoadTexture("button_push.png", TextureList::ButtonGameStartPush);
	LoadTexture("balloon.png", TextureList::Balloon);
	LoadTexture("Reimu.png", TextureList::Stand);
	LoadTexture("UIWindow.png", TextureList::UIWindow);
	LoadTexture("char_th0012.png", TextureList::Reimu);
	LoadTexture("Torigoya.png", TextureList::Torigoya);

	InitButton();
	LoadButton(ButtonList::GameStart,
		{ 0, WINDOW_HEIGHT - 50, 100, WINDOW_HEIGHT },
		TextureList::ButtonGameStart,
		TextureList::ButtonGameStartPush,
		SoundFile::ButtonPush);

	InitUIWindow();
	LoadUIWindow(UIWindowList::Item, g_UIWindowPoint[UIWindowList::Item], TextureList::UIWindow);

	// Player
	g_PointPlayer = { 200, 500 };
	g_DirectionPlayer = Direction::ToRight;
	g_PlayerHungry = 100.0f;

	// 鳥小屋
	g_TorigoyaVisible = false;
}

void Procedure() {

	// プレイヤーの移動
	if (OnKeyboardPush(DIK_RIGHT)) {
		int tmp;

		tmp = (int)(GetTexture(TextureList::Reimu)->Width * (1.0 / 12.0));

		if (g_PointPlayer.x <= WINDOW_WIDTH + tmp){
			g_PointPlayer.x += 1;
			g_DirectionPlayer = Direction::ToRight;
		}
	}
	if (OnKeyboardPush(DIK_LEFT)) {
		if (g_PointPlayer.x >= 0) {
			g_PointPlayer.x -= 1;
			g_DirectionPlayer = Direction::ToLeft;
		}
	}

	// プレイヤーの満腹度更新
	if (g_PlayerHungry > 0) g_PlayerHungry -= 1.0f;
	else g_PlayerHungry = 0.0f;

	if (OnKeyboardDown(DIK_X)) {
		if (g_TorigoyaVisible) {

			if (g_PlayerHungry <= 99.0f) g_PlayerHungry += 10.0f;
			else g_PlayerHungry = 100.0f;
		}
	}

	// 鳥小屋の表示
	if (OnKeyboardDown(DIK_Z)) {
		if (!g_TorigoyaVisible) g_TorigoyaVisible = true;
	}

	// Window(UI)の表示・非表示切り替え
	if(OnKeyboardDown(DIK_I)) {
		g_UIWindowVisible[UIWindowList::Item] = !g_UIWindowVisible[UIWindowList::Item];
	}

	// マウスを離したかを確認
	if (OnMouseUp(MouseButton::Left)) {
		for (int i = 0; i < UIWindowList::UIWindowMax; i++) {
			ResetClicked((UIWindowList)i);
		}
	}

	// ウィンドウ(UI)のドラッグの計算
	for (int i = 0; i < UIWindowList::UIWindowMax; i++) {
		if (IsClicked((UIWindowList)i)) {
			Move((UIWindowList)i, mouse_vector);
		}
	}


	// クリックされたかを計算
	for (int i = 0; i < UIWindowList::UIWindowMax; i++) {
		Texture* texture = GetTexture(g_UIWindowList[i].texture_id);
		RECT rect = { g_UIWindowList[i].point.x ,
			g_UIWindowList[i].point.y
			,g_UIWindowList[i].point.x + texture->Width
			,g_UIWindowList[i].point.y + texture->Height };

		if(OnMouseDown(MouseButton::Left) && MousePosInTheRect(rect) &&	g_UIWindowVisible[i]) {
			SetClicked((UIWindowList)i);
		}
	}


	// SEの出力
	if (OnMouseDown(MouseButton::Left)) g_flgSEPlay[SoundFile::Click] = true;
	if (IsClicked(ButtonList::GameStart)) g_flgSEPlay[SoundFile::ButtonPush] = true;


	// BGM切り替えの検出
	g_PrevSoundBGM = g_SoundBGM;
	if (IsClicked(ButtonList::GameStart)) {
		g_ButtonVisible[ButtonList::GameStart] = false;
		g_SoundBGM = SoundFile::TestBGM2;
	}

}