#pragma once

#define WINDOW_CLASS_NAME TEXT("CreateWindow")
#define WINDOW_WIDTH 860
#define WINDOW_HEIGHT 640

// SoundFile
enum SoundFile
{
	TestBGM,
	TestBGM2,
	Click,
	ButtonPush,
	SoundFileMax,
};

/** @brief タイトル用テクスチャのリスト */
enum TextureList
{
	Background,	//!< 背景
	ButtonGameStart,
	ButtonGameStartPush,
	Balloon,
	Stand,
	UIWindow,
	Reimu,
	Torigoya,
	TextureMax
};

// Button
enum ButtonList
{
	GameStart,
	ButtonMax
};

#define BALLOON_X 500
#define BALLOON_Y 200
#define RECT_BALLOON_NAME { 505, 205, 700, 220 }

// Window(UI)
enum UIWindowList
{
	Item,
	UIWindowMax
};

// Direction
enum Direction {
	ToLeft,
	ToRight
};
