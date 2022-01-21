#pragma once

#include <Windows.h>

enum MouseButton
{
	Left,		//!< ¶
	Right,		//!< ‰E
	Center,		//!< ^‚ñ’†
};

BOOL InitMouse();

void ReleaseMouse();

void UpdateMouse();

bool OnMouseDown(MouseButton button_type);
bool OnMousePush(MouseButton button_type);
bool OnMouseUp(MouseButton button_type);
bool MousePosInTheRect(RECT rect);