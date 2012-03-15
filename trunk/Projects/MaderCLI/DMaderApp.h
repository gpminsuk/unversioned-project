#pragma once

#include "CWindowApp.h"

class DMaderApp : public CWindowApp
{
public:
	DMaderApp();
	~DMaderApp();

	virtual bool CreateApp(TApplicationInfo& Info);
	virtual void Initialize() {};
	virtual void Do();
	virtual bool Tick(unsigned long Time);
	virtual bool DestroyApp() { return true; };
	virtual void SetMousePos(float X, float Y, bool isRatio = false) {};
};
