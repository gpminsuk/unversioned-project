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

HWND DMaderApp::CreateMaderRenderView(HWND ParenthWnd)
{
	m_WindowInfo.m_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= Proc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_WindowInfo.m_hInstance;
	wcex.hIcon			= LoadIcon(m_WindowInfo.m_hInstance, 0);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= _T("CLASS NAME");
	wcex.hIconSm		= LoadIcon(m_WindowInfo.m_hInstance, 0);

	RegisterClassEx(&wcex);

	m_WindowInfo.m_wWidth = 800;
	m_WindowInfo.m_wHeight = 600;

	m_WindowInfo.m_hWnd = ::CreateWindow(
		_T("CLASS NAME"),
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

bool DMaderApp::CreateApp(TApplicationInfo& Info)
{
	m_WindowInfo = (TWindowInfo&)Info;

	m_MaderMainCLI = gcnew MaderMainCLI(this);
	
	return true;
}

void DMaderApp::Do()
{
	m_MaderMainCLI->Run();
}