#include "StdAfx.h"
#include "BViewport.h"
#include "BRenderer.h"

#include "CWindowApp.h"
#include "CDirectXDriver.h"

bool CWindowApp::CreateWindowApp()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= Proc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= LoadIcon(m_hInstance, 0);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= _T("CLASS NAME");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, 0);

	RegisterClassEx(&wcex);

	m_wWidth = 800;
	m_wHeight = 600;
	
	m_hWnd = ::CreateWindow(
		_T("CLASS NAME"),
		_T("CAPTION"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_wWidth,
		m_wHeight,
		0,
		0,
		m_hInstance,
		0);

	m_pViewport = new BViewport();

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	m_pRenderer = new BRenderer();
	m_pRenderer->AddViewport(m_pViewport);

	m_pDriver = new CDirectXDriver(this);

	m_pRenderer->SetDriver(m_pDriver);
	m_pRenderer->SetApplication(this);

	m_pRenderer->Start();
	return true;
}

bool CWindowApp::DestroyWindowApp()
{
	delete m_pRenderer;
	m_pRenderer = 0;
	delete m_pViewport;
	m_pViewport = 0;
	delete m_pDriver;
	m_pDriver = 0;
	return true;
}

void CWindowApp::Do()
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
			// TODI : Do World Tick
		}
	}
}

LRESULT CALLBACK CWindowApp::Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{	
	switch (Message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return 0;
}