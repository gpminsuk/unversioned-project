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

struct TWindowInfo : public TApplicationInfo
{
public:
	TWindowInfo() :
		m_wWidth(800),
		m_wHeight(600)
	{
		Platform = Platform_Windows_x86;
	}

	HWND m_hWnd;
	HINSTANCE m_hInstance;

	int m_wWidth;
	int m_wHeight;
};

class CWindowApp : public BApplication
{
public:
	TWindowInfo m_WindowInfo;

	UMouseMap m_MouseMap;
	TIntPoint m_MousePt;

	static CWindowApp* StaticThis;

	static LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	bool CreateApp(TApplicationInfo& Info);
	void Do();
	void Tick(unsigned long Time);
	bool DestroyApp();

	void MessageTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
	void MouseEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
	void KeyEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam);

	virtual void SetMousePos(float X, float Y, bool isRatio = false);
	virtual void Initialize();
};

extern SYSTEM_INFO GSystemInformation;