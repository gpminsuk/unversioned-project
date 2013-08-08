#pragma once

#include "CWindowApp.h"
#include <vcclr.h>

ref class MosesMainCLI;

class DMosesApp : public CWindowApp
{
	DECLARE_CLASS(DMosesApp,)
public:
	DMosesApp();
	~DMosesApp();

	gcroot<MosesMainCLI^> m_MosesMainCLI;

	HWND CreateMosesRenderView(HWND ParenthWnd);
	virtual bool CreateApp(TApplicationInfo& Info);
	virtual void Do();
};
