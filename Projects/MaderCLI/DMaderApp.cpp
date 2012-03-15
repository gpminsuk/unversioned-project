#include "StdAfx.h"
#include "DMaderApp.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "CDirectXDriver.h"
#include "CWaveIODriver.h"
#include "CCameraViewport.h"
#include "CBox.h"
#include "CDirectionalLight.h"
#include "RResourceManager.h"

#define generic GENERIC
#include "UWorld.h"
#undef GENERIC

DMaderApp::DMaderApp()
{

}

DMaderApp::~DMaderApp()
{
	delete Box;
	delete DirectionalLight;
}

bool DMaderApp::CreateApp(TApplicationInfo& Info)
{
	m_WindowInfo = (TWindowInfo&)Info;

	TDXWindowInfo DXWindowInfo;
	DXWindowInfo.m_hWnd = m_WindowInfo.m_hWnd;
	DXWindowInfo.m_wHeight = m_WindowInfo.m_wHeight;
	DXWindowInfo.m_wWidth = m_WindowInfo.m_wWidth;

	GDriver = new CDirectXDriver(DXWindowInfo);
	GSoundDriver = new CWaveIODriver();
	if(!GDriver->CreateDriver())
		return false;

	m_pRenderer = new BRenderer(this);

	m_pWorld = new UWorld();
	m_pViewport = new CCameraViewport();
	m_pRenderer->AddViewport(m_pViewport);
	m_pWorld->AddViewport(m_pViewport);

	RResourceManager::LoadResources();

	if(m_pRenderer)
	{
		bRenderThreadQuit = false;
		m_pRenderer->Start();
	}
	else
	{
		bRenderThreadQuit = true;
	}

	if(m_pWorld)
	{
		m_pWorld->InitializeWorld();
	}

	Box = new CBox(SideType_Inside);
	Box->m_Scale = TVector3(100.0f,100.0f,100.0f);
	Box->UpdateTransform();
	m_pWorld->AddThing(Box);

	DirectionalLight = new CDirectionalLight();
	DirectionalLight->m_Location = TVector3(0.0f, 100.0f, 0.0f);
	DirectionalLight->UpdateTransform();
	m_pWorld->AddThing(DirectionalLight);

	return true;
}

void DMaderApp::Do()
{
	while(!bQuit)
	{
		Sleep(1000);
	}
}

bool DMaderApp::Tick(unsigned long Time)
{
	if(m_pViewport)
	{
		m_pViewport->UpdateViewport();
	}
	if(m_pWorld)
	{
		m_pWorld->Tick(Time);
	}
	return true;
}