#pragma once

#include "..\AA\Src\TWindowInfo.h"
#include "..\AA\Src\BApplication.h"

class CMaderApp : public BApplication
{
public:
	int m_wWidth;
	int m_wHeight;

	TWindowInfo WindowInfo;

	bool CreateMaderApp();

	virtual bool CreateApp() { return true; };
	virtual void Do();
	virtual bool DestroyApp() { return true; };
	virtual void SetMousePos(float X, float Y, bool isRatio = false) {};
};
