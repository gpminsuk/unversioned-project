#include "StdAfx.h"
#include "CMaderApp.h"
#include "..\AA\Src\BRenderer.h"
#include "..\AA\Src\CDirectXDriver.h"
#include "..\AA\Src\RResourceManager.h"

#define generic GENERIC
#include "..\AA\Src\World\UWorld.h"
#undef GENERIC

CMaderApp::CMaderApp()
{

}

bool CMaderApp::CreateApp(TApplicationInfo& Info)
{
	m_WindowInfo = (TCSharpWindowInfo&)Info;

	TDXWindowInfo DXWindowInfo;
	DXWindowInfo.m_hWnd = m_WindowInfo.m_hWnd;
	DXWindowInfo.m_wHeight = m_WindowInfo.m_wHeight;
	DXWindowInfo.m_wWidth = m_WindowInfo.m_wWidth;

	//GDriver = new CDirectXDriver(DXWindowInfo);
	//if(!GDriver->CreateDriver())
	//	return false;

	//RResourceManager::LoadResources();

	//m_pRenderer = new BRenderer(this);

	//m_pWorld = new UWorld();
	//m_pWorld->InitializeWorld();

	//m_pRenderer->Start();
	return true;
}

void CMaderApp::Do()
{
	MSG msg;

	while(!bQuit)
	{
		Sleep(1000);	
	}
}