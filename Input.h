#pragma once

#include <Windows.h>

#include "Mouse.h"
#include "Keyboard.h"

BOOL InitInput();

void ReleaseInput();

void UpdateInput();