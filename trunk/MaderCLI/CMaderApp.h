#pragma once

#include "..\AA\Src\TWindowInfo.h"
#include "..\AA\Src\BApplication.h"

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

	virtual bool CreateApp(TApplicationInfo& Info);
	virtual void Initialize() {};
	virtual void Do();
	virtual bool DestroyApp() { return true; };
	virtual void SetMousePos(float X, float Y, bool isRatio = false) {};
};
