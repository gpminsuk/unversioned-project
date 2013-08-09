#include "StdAfx.h"
#include "CWindowsViewport.h"

#include "BCamera.h"
#include "BDriver.h"

#include "CWindowApp.h"
#include "CUIButton.h"

CWindowsViewport::CWindowsViewport(unsigned int Width, unsigned int Height, HWND InHandle) 
	: BViewport(Width, Height),
	Handle(InHandle) {
	SwapChain = GDriver->CreateSwapChain(this);
}

bool CWindowsViewport::Tick(unsigned long Time) {
    return true;
}

void CWindowsViewport::SetProperty(TString& PropertyName, TString& Value) {

}
