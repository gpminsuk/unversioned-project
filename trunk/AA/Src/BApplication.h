#pragma once

class BRenderer;
class UWorld;

enum EPlatform
{
	Platform_Windows_x86
};

struct TApplicationInfo
{
public:
	TApplicationInfo() :
	  Platform(Platform_Windows_x86)
	  {}

	EPlatform Platform;
};

class BApplication
{
public:
	BApplication(void);
	virtual ~BApplication();

	BRenderer *m_pRenderer;

	UWorld*	m_pWorld;

	bool bQuit;
	bool bRenderThreadQuit;

	virtual void SetMousePos(float X, float Y, bool isRatio = false) = 0;

	virtual bool CreateApp(TApplicationInfo& Info) = 0;
	virtual void Do() = 0;
	virtual bool DestroyApp() = 0;
	virtual void Initialize() = 0;
};
