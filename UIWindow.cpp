#include "UI_Window.h"
#include "Const.h"
#include "Parameter.h"
#include "Texture.h"
#include <Windows.h>

bool IsRangeCheckUIWindow(int window_id) {
	if (window_id >= 0 && window_id < UIWindowList::UIWindowMax) {
	}
	else return false;
	return true;
}

void InitUIWindow() {
	for (int i = 0; i < UIWindowList::UIWindowMax; i++) {
		g_UIWindowList[i].point = { 0,0 };
		g_UIWindowList[i].isClicked = false;

		g_UIWindowVisible[i] = false;
	}

	g_UIWindowPoint[UIWindowList::Item] = { 400, 100 };
}
bool LoadUIWindow(UIWindowList window_id, POINT p, TextureList t_id) {
	if (IsRangeCheckUIWindow(window_id)) {
		g_UIWindowList[window_id].point = p;
		g_UIWindowList[window_id].texture_id = t_id;
		g_UIWindowList[window_id].isClicked = false;
	}
	else return false;

	return true;
}

void DrawUIWindow(UIWindowList window_id) {
	DrawTexture(g_UIWindowList[window_id].point.x,
		g_UIWindowList[window_id].point.y,
		GetTexture(g_UIWindowList[window_id].texture_id));
}

void SetClicked(UIWindowList window_id) {
	if (IsRangeCheckUIWindow(window_id)) {
		g_UIWindowList[window_id].isClicked = true;
	}
}

void ResetClicked(UIWindowList window_id) {
	if (IsRangeCheckUIWindow(window_id)) {
		g_UIWindowList[window_id].isClicked = false;
	}
}

bool IsClicked(UIWindowList window_id) {
	if (IsRangeCheckUIWindow(window_id)) {
		return g_UIWindowList[window_id].isClicked;
	}
	else return false;
}

void Move(UIWindowList window_id, POINT vector) {
	if (IsRangeCheckUIWindow(window_id)) {
		g_UIWindowList[window_id].point.x = g_UIWindowList[window_id].point.x + vector.x;
		g_UIWindowList[window_id].point.y = g_UIWindowList[window_id].point.y + vector.y;
	}
}
