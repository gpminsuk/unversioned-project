#pragma once

#include "InputDefine.h"
#include "TDataTypes.h"
#include "TWindowInfo.h"

#include "AApplication.h"
#include <windows.h>

class CWindowsViewport;

struct UMouseMap
{
	unsigned char bLButtonDown :1;
	unsigned char bRButtonDown :1;
	unsigned char bMButtonDown :1;
};

struct TWindowInfo: public TApplicationInfo
{
public:
	TWindowInfo()
	{
		Platform = Platform_Windows_x86;
	}

	HWND m_hWnd;
	HINSTANCE m_hInstance;

	int m_wWidth;
	int m_wHeight;
};

class CWindowsApplication: public AApplication
{
DECLARE_CLASS(CWindowsApplication,)
	public:
	UMouseMap m_MouseMap;
	TIntPoint m_MousePt;
	TWindowInfo m_WindowInfo;

	static CWindowsApplication* StaticThis;

	CWindowsApplication();

	static LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	
	bool CreateApplicationWindow(TApplicationInfo& Info);
	void Do();
	bool Tick(unsigned long Time);
	bool DestroyApp();

	BViewport* FindViewport(HWND hWnd);

	virtual void MessageTranslator(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
	void MouseEventTranslator(BViewport* Viewport, UINT Message, WPARAM wParam, LPARAM lParam);
	void KeyEventTranslator(BViewport* Viewport, UINT Message, WPARAM wParam, LPARAM lParam);

	virtual void InputMouse(BViewport* Viewport, EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(BViewport* Viewport, EKey_Event Event, TKeyInput_Param& Param);

	virtual void SetMousePos(float X, float Y, bool isRatio = false);
};

extern SYSTEM_INFO GSystemInformation;
