#include "StdAfx.h"
#include "CMaderApp.h"
#include "..\Src\BRenderer.h"
#include "..\Src\CDirectXDriver.h"

bool CMaderApp::CreateMaderApp()
{
	m_pRenderer = new BRenderer();

	GDriver = new CDirectXDriver(&WindowInfo);
	GDriver->CreateDriver();

	m_pRenderer->SetApplication(this);

	m_pRenderer->Start();
	return true;
}

void CMaderApp::Do()
{
	MSG msg;
	while(!bQuit)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);			
		}
		else
		{
			// TODI : Do World Tick
		}
	}
}