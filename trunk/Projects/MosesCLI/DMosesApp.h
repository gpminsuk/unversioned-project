#pragma once

#include "CWindowsApplication.h"
#include <vcclr.h>

ref class MosesMainCLI;

class DMosesApp : public CWindowsApplication
{
	DECLARE_CLASS(DMosesApp,)
public:
	DMosesApp();
	~DMosesApp();

	gcroot<MosesMainCLI^> m_MosesMainCLI;

	HWND CreateViewportWindow(BViewport* Viewport, int Width, int Height, HWND ParentHandle);
	virtual bool CreateApplicationWindow(TApplicationInfo& Info);
	virtual void Do();
};
