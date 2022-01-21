#include "Button.h"
#include "Const.h"
#include <Windows.h>
#include "Mouse.h"
#include "Texture.h"
#include "Parameter.h"

bool IsRangeCheckButton(int i);

bool IsRangeCheckButton(int i) {
	if (i >= 0 && i < ButtonList::ButtonMax) {
	}
	else return false;

	return true;
}

bool InitButton() {
	for (int i = 0; i < ButtonList::ButtonMax; i++) g_ButtonVisible[i] = false;
	g_ButtonVisible[ButtonList::GameStart] = true;

	return true;
}

bool LoadButton(ButtonList btn_id, RECT r, TextureList t, TextureList t_p, SoundFile s_p) {
	if (IsRangeCheckButton(btn_id)) {
		g_ButtonList[btn_id].rect = r;
		g_ButtonList[btn_id].texture = t;
		g_ButtonList[btn_id].texture_push = t_p;
		g_ButtonList[btn_id].sound_push = s_p;
	}
	else return false;

	return true;
}

void DrawButton(ButtonList btn_id) {
	if(IsClicking(btn_id)){
		DrawTexture(g_ButtonList[btn_id].rect, GetTexture(g_ButtonList[btn_id].texture_push));
	}
	else {
		DrawTexture(g_ButtonList[btn_id].rect, GetTexture(g_ButtonList[btn_id].texture));
	}
}

bool IsClicking(ButtonList btn_id) {
	if (IsRangeCheckButton(btn_id)) {}
	else return false;
	return g_ButtonVisible[btn_id] && OnMousePush(MouseButton::Left) && MousePosInTheRect(g_ButtonList[btn_id].rect);
}

bool IsClicked(ButtonList btn_id) {
	if (IsRangeCheckButton(btn_id)) {}
	else return false;
	return g_ButtonVisible[btn_id] && OnMouseUp(MouseButton::Left) && MousePosInTheRect(g_ButtonList[btn_id].rect);
}
