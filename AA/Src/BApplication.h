#pragma once

class BRenderer;
class UWorld;

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

	virtual bool CreateApp() = 0;
	virtual void Do() = 0;
	virtual bool DestroyApp() = 0;
};
