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
#include "MaderMainCLI.h"

#define generic GENERIC
#include "UWorld.h"
#undef GENERIC

IMPLEMENT_CLASS(DMaderApp);

DMaderApp::DMaderApp()
{

}

DMaderApp::~DMaderApp()
{
}

bool DMaderApp::CreateApp(TApplicationInfo& Info)
{
	m_MaderMainCLI = gcnew MaderMainCLI(this);
	return true;
}

void DMaderApp::Do()
{
	m_MaderMainCLI->Run();
}