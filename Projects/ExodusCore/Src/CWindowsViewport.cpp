#include "StdAfx.h"
#include "CWindowsViewport.h"

#include "BCamera.h"
#include "BDriver.h"

#include "CWindowsApplication.h"
#include "CUIButton.h"

CWindowsViewport::CWindowsViewport(unsigned int Width, unsigned int Height, EViewportProjectionType InProjectionType, EViewportRenderMode InRenderMode, ECameraMode InCameraMode, HWND InHandle) 
	: BViewport(Width, Height, InProjectionType, InRenderMode, InCameraMode),
	Handle(InHandle),
	IsMainViewport(false) {
}

bool CWindowsViewport::Tick(unsigned long Time) {
    return true;
}

void CWindowsViewport::SetProperty(TString& PropertyName, TString& Value) {

}

void CWindowsViewport::OnSizeChanged() {
	RECT r;
	::GetClientRect(Handle, &r);
	Width = r.right;
	Height = r.bottom;
}