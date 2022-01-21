#pragma once

#include <Windows.h>

BOOL InitKeyboard();
void ReleaseKeyboard();

void KeyUpdate();

bool OnKeyboardDown(int keycode);
bool OnKeyboardPush(int keycode);
bool OnKeyboardUp(int keycode);
