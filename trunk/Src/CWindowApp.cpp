#include "StdAfx.h"
#include "BRenderer.h"

#include "InputDefine.h"

#include "UWorld.h"

#include "CWindowApp.h"
#include "CDirectXDriver.h"

CWindowApp	*GApp = 0;
unsigned char GKeyMap[KEYMAP_SIZE] = {0,};

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

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	m_pRenderer = new BRenderer();

	GDriver = new CDirectXDriver(&m_WindowInfo);
	GDriver->CreateDriver();

	m_pRenderer->SetApplication(this);

	m_pRenderer->Start();

	m_pWorld = new UWorld(m_pRenderer);
	m_pWorld->InitializeWorld();

	GApp = this;

	m_MouseMap.bLButtonDown = 0;
	m_MouseMap.bRButtonDown = 0;
	m_MouseMap.bMButtonDown = 0;

	return true;
}

bool CWindowApp::DestroyWindowApp()
{
	delete m_pRenderer;
	m_pRenderer = 0;
	delete GDriver;
	GDriver = 0;
	m_pWorld->DestroyWorld();
	delete m_pWorld;
	m_pWorld = 0;
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
			m_pWorld->Tick(0);
			// TODI : Do World Tick
		}
	}
}

void CWindowApp::MouseEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam)
{
	TMouseInput_Param Param;
	Param.bRAltDown = ::GetKeyState(VK_RMENU);
	Param.bLAltDown = ::GetKeyState(VK_LMENU);
	Param.bRCtrlDown = ::GetKeyState(VK_RCONTROL);
	Param.bLCtrlDown = ::GetKeyState(VK_LCONTROL);
	Param.bRShiftDown = ::GetKeyState(VK_RSHIFT);
	Param.bLShiftDown = ::GetKeyState(VK_LSHIFT);
	Param.bLButtonDown = m_MouseMap.bLButtonDown;
	Param.bRButtonDown = m_MouseMap.bRButtonDown;
	Param.bMButtonDown = m_MouseMap.bMButtonDown;
	Param.X = LOWORD(lParam);
	Param.Y = HIWORD(lParam);
	if(m_MousePt.x) 
		Param.dX = m_MousePt.x - Param.X;
	else
		Param.dX = 0;
	if(m_MousePt.y)
		Param.dY = m_MousePt.y - Param.Y;
	else
		Param.dY = 0;
	m_MousePt.x = Param.X;
	m_MousePt.y = Param.Y;
	switch(Message)
	{
	case WM_LBUTTONDBLCLK:
		m_pWorld->InputMouse(MOUSE_LeftButtonDblClk, Param);
		break;
	case WM_RBUTTONDBLCLK:
		m_pWorld->InputMouse(MOUSE_RightButtonDblClk, Param);
		break;
	case WM_MBUTTONDBLCLK:
		m_pWorld->InputMouse(MOUSE_MiddleButtonDblClk, Param);
		break;
	case WM_LBUTTONDOWN:
		m_MouseMap.bLButtonDown = true;
		m_pWorld->InputMouse(MOUSE_LeftButtonDown, Param);
		break;
	case WM_RBUTTONDOWN:
		m_MouseMap.bRButtonDown = true;
		m_pWorld->InputMouse(MOUSE_RightButtonDown, Param);
		break;
	case WM_MBUTTONDOWN:
		m_MouseMap.bMButtonDown = true;
		m_pWorld->InputMouse(MOUSE_MiddleButtonDown, Param);
		break;
	case WM_LBUTTONUP:
		m_MouseMap.bLButtonDown = false;
		m_pWorld->InputMouse(MOUSE_LeftButtonUp, Param);
		break;
	case WM_RBUTTONUP:
		m_MouseMap.bRButtonDown = false;
		m_pWorld->InputMouse(MOUSE_RightButtonUp, Param);
		break;
	case WM_MBUTTONUP:
		m_MouseMap.bMButtonDown = false;
		m_pWorld->InputMouse(MOUSE_MiddleButtonUp, Param);
		break;
	case WM_MOUSEWHEEL:
		Param.delta = HIWORD(wParam);
		m_pWorld->InputMouse(MOUSE_Wheel, Param);
		break;
	case WM_MOUSEMOVE:
		m_pWorld->InputMouse(MOUSE_Move, Param);
		break;
	}
}

void CWindowApp::KeyEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam)
{
	TKeyInput_Param Param;
	Param.bRAltDown = ::GetKeyState(VK_RMENU);
	Param.bLAltDown = ::GetKeyState(VK_LMENU);
	Param.bRCtrlDown = ::GetKeyState(VK_RCONTROL);
	Param.bLCtrlDown = ::GetKeyState(VK_LCONTROL);
	Param.bRShiftDown = ::GetKeyState(VK_RSHIFT);
	Param.bLShiftDown = ::GetKeyState(VK_LSHIFT);
	Param.Key = (unsigned short)wParam;	
	switch(Message)
	{
	case WM_KEYDOWN:
		GKeyMap[Param.Key] = 0x01;
		m_pWorld->InputKey(KEY_Down, Param);
		break;
	case WM_KEYUP:
		GKeyMap[Param.Key] = 0x00;
		m_pWorld->InputKey(KEY_Up, Param);
		break;
	}
}

void CWindowApp::MessageTranslator(UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
		MouseEventTranslator(Message, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		KeyEventTranslator(Message, wParam, lParam);
		break;
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
		if(GApp)
			GApp->MessageTranslator(Message, wParam, lParam);
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return 0;
}