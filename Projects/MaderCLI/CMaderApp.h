#pragma once

#include "..\Unique\Src\TDataTypes.h"
#include "..\Unique\Src\TWindowInfo.h"
#include "..\Unique\Src\BApplication.h"

struct UMouseMap
{
	unsigned char bLButtonDown:1;
	unsigned char bRButtonDown:1;
	unsigned char bMButtonDown:1;
};

struct TCSharpWindowInfo : public TApplicationInfo
{
public:
	TCSharpWindowInfo() :
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

class CMaderApp : public BApplication
{
public:
	CMaderApp();

	TCSharpWindowInfo m_WindowInfo;

	UMouseMap m_MouseMap;
	TIntPoint m_MousePt;

	virtual bool CreateApp(TApplicationInfo& Info);
	virtual void Initialize() {};
	virtual void Do();
	virtual void Tick(unsigned long Time);
	virtual bool DestroyApp() { return true; };
	virtual void SetMousePos(float X, float Y, bool isRatio = false) {};

	void MessageTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
	void MouseEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
	void KeyEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam);
};
