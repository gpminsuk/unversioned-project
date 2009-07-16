#include "StdAfx.h"
#include "CMaderApp.h"
#include "..\Src\BRenderer.h"
#include "..\Src\CDirectXDriver.h"

bool CMaderApp::CreateMaderApp()
{
	m_pRenderer = new BRenderer();

	GDriver = new CDirectXDriver(this);
	GDriver->CreateDriver();

	m_pRenderer->SetApplication(this);

	m_pRenderer->Start();
	return true;
}