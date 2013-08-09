#pragma once

#include <windows.h>

#include "TDataTypes.h"
#include "BViewport.h"

class BCamera;

class CWindowsViewport: public BViewport
{
public:
	CWindowsViewport(unsigned int Width, unsigned int Height, HWND hWnd);

	HWND Handle;

	virtual bool Tick(unsigned long Time);
	virtual void SetProperty(TString& PropertyName, TString& Value);
};
