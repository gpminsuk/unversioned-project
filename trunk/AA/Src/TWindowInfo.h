#pragma once

#include <Windows.h>

struct TWindowInfo
{
	HWND m_hWnd;
	HINSTANCE m_hInstance;

	int m_wWidth;
	int m_wHeight;
};