#pragma once

#include <Windows.h>
#include "Const.h"

struct UI_Window {
	POINT point;
	TextureList texture_id;
	bool isClicked;
};

void InitUIWindow();
bool LoadUIWindow(UIWindowList window_id, POINT p, TextureList t_id);
void DrawUIWindow(UIWindowList window_id);
void SetClicked(UIWindowList window_id);
void ResetClicked(UIWindowList window_id);
bool IsClicked(UIWindowList window_id);
void Move(UIWindowList window_id, POINT vector);
