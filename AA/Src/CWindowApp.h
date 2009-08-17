#pragma once

#include "TDataTypes.h"
#include "TWindowInfo.h"

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
	TWindowInfo m_WindowInfo;

	UMouseMap m_MouseMap;
	TIntPoint m_MousePt;

	static LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	bool CreateWindowApp();
	void Do();
	bool DestroyWindowApp();

	void MessageTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
	void MouseEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
	void KeyEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam);

	virtual void SetMousePos(float X, float Y, bool isRatio = false);
};

extern CWindowApp* GApp;