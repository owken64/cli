#pragma once

#include <Windows.h>
#include "Const.h"

struct Button {
	RECT rect;
	TextureList texture;
	TextureList texture_push;
	SoundFile   sound_push;
};

bool InitButton();
bool LoadButton(ButtonList btn_id, RECT r, TextureList t, TextureList t_p, SoundFile s_p);
void DrawButton(ButtonList btn_id);

bool IsClicking(ButtonList btn_id);
bool IsClicked(ButtonList btn_id);

