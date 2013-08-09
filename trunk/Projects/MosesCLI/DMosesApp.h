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

	TArray<TWindowInfo> WindowInfos;

	HWND CreateMosesRenderView(int Width, int Height, HWND ParenthWnd);
	void ResizeMosesRenderView(HWND Handle, int left, int top, int right, int bottom);
	virtual bool CreateApp(TApplicationInfo& Info);
	virtual void Do();
};
