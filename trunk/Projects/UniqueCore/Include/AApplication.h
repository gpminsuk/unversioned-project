#pragma once

class ASystemInfo;

class BRenderer;
class BViewport;
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

class AApplication
{
public:
	AApplication(void);
	virtual ~AApplication();

	ASystemInfo* SystemInfo;

	BRenderer *m_pRenderer;
	BViewport *m_pViewport;

	UWorld*	m_pWorld;

	bool bQuit;
	bool bRenderThreadQuit;

	virtual void SetMousePos(float X, float Y, bool isRatio = false) = 0;

	virtual bool CreateApp(TApplicationInfo& Info) = 0;
	virtual void Do() = 0;
	virtual void Tick(unsigned long Time) = 0;
	virtual bool DestroyApp() = 0;
	virtual void Initialize() = 0;
};
