#include "StdAfx.h"
#include "DMosesApp.h"
#include "BRenderPass.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "CDirectXDriver.h"
#include "CWaveIODriver.h"
#include "CWindowsViewport.h"
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

HWND DMosesApp::CreateViewportWindow(BViewport* Viewport, int Width, int Height, HWND ParentHandle) {
	CWindowsViewport* WindowViewport = dynamic_cast<CWindowsViewport*>(Viewport);	
	WindowViewport->Width = Width;
	WindowViewport->Height = Height;
	WindowViewport->Handle = ::CreateWindow(
		_T("static"),
		_T(""),
		WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WindowViewport->Width,
		WindowViewport->Height,
		ParentHandle,
		0,
		m_WindowInfo.m_hInstance,
		0);	
	return WindowViewport->Handle;
}

bool DMosesApp::CreateApplicationWindow(TApplicationInfo& Info) {
	m_WindowInfo = (TWindowInfo&)Info;

	m_MosesMainCLI = gcnew MosesMainCLI(this);
	m_MosesMainCLI->MainWindow->Width = m_WindowInfo.m_wWidth;
	m_MosesMainCLI->MainWindow->Height = m_WindowInfo.m_wHeight;
	m_MosesMainCLI->MainWindow->ShowWindow();

	m_WindowInfo.m_hWnd = (HWND)m_MosesMainCLI->MainWindow->GetWindowHandle().ToPointer();

	GDriver->CreateDriver(this);
	m_pRenderer = new BRenderer(this);

	RResourceManager::LoadResources();
	GRenderPassResource.Initialize(m_WindowInfo.m_wWidth, m_WindowInfo.m_wHeight);

	bRenderThreadQuit = false;
	m_pRenderer->Start();
	return true;
}

void DMosesApp::Do()
{
	m_MosesMainCLI->Run();
}