#pragma once

#include "BApplication.h"
#include <windows.h>

class CWindowApp : public BApplication
{
public:
	HWND m_hWnd;
	HINSTANCE m_hInstance;

	int m_wWidth;
	int m_wHeight;

	static LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	bool CreateWindowApp();
	void Do();
	bool DestroyWindowApp();
};