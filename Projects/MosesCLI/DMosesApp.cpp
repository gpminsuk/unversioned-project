#include "StdAfx.h"
#include "DMosesApp.h"
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

void DMosesApp::ResizeMosesRenderView(HWND Handle, int left, int top, int right, int bottom) {
	::MoveWindow(Handle, left, top, right - left, bottom - top, true);
}

HWND DMosesApp::CreateMosesRenderView(int Width, int Height, HWND ParenthWnd)
{
	TWindowInfo Info;
	Info.m_hInstance = GetModuleHandle(NULL);

	Info.m_wWidth = Width;
	Info.m_wHeight = Height;

	Info.m_hWnd = ::CreateWindow(
		_T("static"),
		_T(""),
		WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Info.m_wWidth,
		Info.m_wHeight,
		ParenthWnd,
		0,
		m_WindowInfo.m_hInstance,
		0);
	WindowInfos.AddItem(Info);
	return Info.m_hWnd;
}

bool DMosesApp::CreateApp(TApplicationInfo& Info)
{
	m_WindowInfo = (TWindowInfo&)Info;

	m_MosesMainCLI = gcnew MosesMainCLI(this);
	m_MosesMainCLI->MainWindow->ShowWindow();
	
	m_WindowInfo = WindowInfos(0);
	m_pViewport = new CWindowsViewport(m_WindowInfo.m_wWidth, m_WindowInfo.m_wHeight, m_WindowInfo.m_hWnd);
	GDriver->CreateDriver(m_pViewport);
	m_pRenderer = new BRenderer(this);
	m_pRenderer->AddViewport(m_pViewport);
	m_pWorld->m_pRenderer = m_pRenderer;
	for(unsigned int i=1;i<WindowInfos.Size();++i) {
		BViewport* Viewport = new CWindowsViewport(WindowInfos(i).m_wWidth, WindowInfos(i).m_wHeight, WindowInfos(i).m_hWnd);
		m_pRenderer->AddViewport(Viewport);
	}
	return true;
}

void DMosesApp::Do()
{
	m_MosesMainCLI->Run();
}