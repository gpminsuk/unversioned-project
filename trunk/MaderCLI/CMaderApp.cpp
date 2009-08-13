#include "StdAfx.h"
#include "CMaderApp.h"
#include "..\AA\Src\BRenderer.h"
#include "..\AA\Src\CDirectXDriver.h"
#include "..\AA\Src\World\UWorld.h"

bool CMaderApp::CreateMaderApp()
{
	m_pRenderer = new BRenderer();

	GDriver = new CDirectXDriver(&WindowInfo);
	GDriver->CreateDriver();

	m_pRenderer->SetApplication(this);

	m_pRenderer->Start();

	m_pWorld = new UWorld(m_pRenderer);
	m_pWorld->InitializeWorld();
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
			m_pWorld->Tick(0);
			// TODI : Do World Tick
		}
	}
}