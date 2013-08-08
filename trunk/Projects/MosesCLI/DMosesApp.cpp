#include "StdAfx.h"
#include "DMosesApp.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "CDirectXDriver.h"
#include "CWaveIODriver.h"
#include "CCameraViewport.h"
#include "CBox.h"
#include "CDirectionalLight.h"
#include "RResourceManager.h"
#include "MosesMainCLI.h"

#define generic GENERIC
#include "UWorld.h"
#undef GENERIC

IMPLEMENT_CLASS(DMosesApp);

DMosesApp::DMosesApp()
{

}

DMosesApp::~DMosesApp()
{
}

HWND DMosesApp::CreateMosesRenderView(HWND ParenthWnd)
{
	m_WindowInfo.m_hInstance = GetModuleHandle(NULL);

	m_WindowInfo.m_wWidth = 800;
	m_WindowInfo.m_wHeight = 600;

	m_WindowInfo.m_hWnd = ::CreateWindow(
		_T("static"),
		_T("CAPTION"),
		WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_WindowInfo.m_wWidth + 14,
		m_WindowInfo.m_wHeight + 36,
		ParenthWnd,
		0,
		m_WindowInfo.m_hInstance,
		0);

	return m_WindowInfo.m_hWnd;
}

bool DMosesApp::CreateApp(TApplicationInfo& Info)
{
	m_WindowInfo = (TWindowInfo&)Info;

	m_MosesMainCLI = gcnew MosesMainCLI(this);
	m_MosesMainCLI->MainWindow->ShowWindow();
	return true;
}

void DMosesApp::Do()
{
	m_MosesMainCLI->Run();
}