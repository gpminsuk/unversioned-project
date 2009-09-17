#include "StdAfx.h"
#include "CMaderApp.h"
#include "..\AA\Src\BRenderer.h"
#include "..\AA\Src\CDirectXDriver.h"
#include "..\AA\Src\RResourceManager.h"

#define generic GENERIC
#include "..\AA\Src\World\UWorld.h"
#undef GENERIC

bool CMaderApp::CreateMaderApp()
{
	GDriver = new CDirectXDriver(&WindowInfo);
	if(!GDriver->CreateDriver())
		return false;

	RResourceManager::LoadResources();

	m_pRenderer = new BRenderer();		

	m_pRenderer->SetApplication(this);

	m_pWorld = new UWorld(m_pRenderer);
	m_pWorld->InitializeWorld();

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
			m_pWorld->Tick(0);
			// TODI : Do World Tick
		}
	}
}