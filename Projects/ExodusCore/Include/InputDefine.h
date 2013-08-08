#pragma once

#define MOUSE_WHEEL_DELTA	120.0f
#define KEYMAP_SIZE			0xFF

enum EMouse_Event
{
	MOUSE_None,
	MOUSE_LeftButtonDown,
	MOUSE_LeftButtonDblClk,
	MOUSE_LeftButtonUp,
	MOUSE_RightButtonDown,
	MOUSE_RightButtonDblClk,
	MOUSE_RightButtonUp,
	MOUSE_MiddleButtonDown,
	MOUSE_MiddleButtonDblClk,
	MOUSE_MiddleButtonUp,
	MOUSE_XButton1Down,
	MOUSE_XButton1DblClk,
	MOUSE_XButton1Up,
	MOUSE_XButton2Down,
	MOUSE_XButton2DblClk,
	MOUSE_XButton2Up,
	MOUSE_Wheel,
	MOUSE_Move,
};

enum EKey_Event
{
	KEY_None,
	KEY_Down,
	KEY_Up
};

struct TInput_Param
{
	unsigned char bLShiftDown :1;
	unsigned char bRShiftDown :1;
	unsigned char bLCtrlDown :1;
	unsigned char bRCtrlDown :1;
	unsigned char bLAltDown :1;
	unsigned char bRAltDown :1;
};

struct TMouseInput_Param: TInput_Param
{
	TMouseInput_Param();

	unsigned char bLButtonDown :1;
	unsigned char bRButtonDown :1;
	unsigned char bMButtonDown :1;

	unsigned short X;
	unsigned short Y;
	short dX;
	short dY;
	short delta;
};

extern unsigned char GKeyMap[KEYMAP_SIZE];

struct TKeyInput_Param: TInput_Param
{
	unsigned short Key;
};
