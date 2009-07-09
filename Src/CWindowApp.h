#pragma once

#include "TDataTypes.h"

#include "BApplication.h"
#include <windows.h>

struct UMouseMap
{
	unsigned char bLButtonDown:1;
	unsigned char bRButtonDown:1;
	unsigned char bMButtonDown:1;
};

class CWindowApp : public BApplication
{
public:
	HWND m_hWnd;
	HINSTANCE m_hInstance;

	int m_wWidth;
	int m_wHeight;

	UMouseMap m_MouseMap;
	TIntPoint m_MousePt;

	static LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	bool CreateWindowApp();
	void Do();
	bool DestroyWindowApp();

	void MessageTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
	void MouseEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
	void KeyEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
};