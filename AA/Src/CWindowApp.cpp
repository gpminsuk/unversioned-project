#include "StdAfx.h"
#include "BRenderer.h"
#include "BViewport.h"

#include "InputDefine.h"

#include "UWorld.h"

#include "CWindowApp.h"

#include "RResourceManager.h"


CWindowApp*	CWindowApp::StaticThis = 0;
SYSTEM_INFO GSystemInformation;
unsigned char GKeyMap[KEYMAP_SIZE] = {0,};

void CWindowApp::Initialize()
{
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
}

bool CWindowApp::CreateApp(TApplicationInfo& Info)
{
	m_WindowInfo = (TWindowInfo&)Info;
	GetSystemInfo(&GSystemInformation);

	StaticThis = this;

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
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_WindowInfo.m_wWidth + 14,
		m_WindowInfo.m_wHeight + 36,
		0,
		0,
		m_WindowInfo.m_hInstance,
		0);

	ShowWindow(m_WindowInfo.m_hWnd, SW_SHOW);
	UpdateWindow(m_WindowInfo.m_hWnd);

	m_MouseMap.bLButtonDown = 0;
	m_MouseMap.bRButtonDown = 0;
	m_MouseMap.bMButtonDown = 0;

	return true;
}

bool CWindowApp::DestroyApp()
{
	if(m_pWorld) m_pWorld->DestroyWorld();
	delete m_pWorld;
	m_pWorld = 0;

	delete m_pRenderer;
	m_pRenderer = 0;

	RResourceManager::ReleaseAllResources();
	return true;
}

void CWindowApp::Do()
{
	MSG msg;
	int Count = 0;
	while(!bQuit)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT || GKeyMap[VK_ESCAPE])
			{
				bQuit = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);			
		}
		else
		{
			m_pViewport->UpdateViewport();
			m_pWorld->Tick(0);
			// TODO : Do World Tick
		}
		//Sleep(1);
	}
	while(!bRenderThreadQuit);
}

void CWindowApp::SetMousePos(float X, float Y, bool isRatio)
{
	POINT MousePt;
	if(isRatio)
	{
		RECT rt;	
		::GetClientRect(m_WindowInfo.m_hWnd, &rt);
		MousePt.x = (LONG)(X*(rt.right - rt.left));
		MousePt.y = (LONG)(Y*(rt.bottom - rt.top));
	}
	else
	{
		MousePt.x = (LONG)X;
		MousePt.y = (LONG)Y;
	}
	m_MousePt.x = MousePt.x;
	m_MousePt.y = MousePt.y;
	::ClientToScreen(m_WindowInfo.m_hWnd, &MousePt);
	::SetCursorPos((int)MousePt.x ,(int)MousePt.y);
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
	if(m_MousePt.x && m_MousePt.y && Param.dX == 0 && Param.dY == 0)
		return;
	m_MousePt.x = Param.X;
	m_MousePt.y = Param.Y;
	EMouse_Event Event;
	switch(Message)
	{
	case WM_LBUTTONDBLCLK:
		Event = MOUSE_LeftButtonDblClk;
		break;
	case WM_RBUTTONDBLCLK:
		Event = MOUSE_RightButtonDblClk;
		break;
	case WM_MBUTTONDBLCLK:
		Event = MOUSE_MiddleButtonDblClk;
		break;
	case WM_LBUTTONDOWN:
		m_MouseMap.bLButtonDown = true;
		Event = MOUSE_LeftButtonDown;
		break;
	case WM_RBUTTONDOWN:
		m_MouseMap.bRButtonDown = true;
		Event = MOUSE_RightButtonDown;
		break;
	case WM_MBUTTONDOWN:
		m_MouseMap.bMButtonDown = true;
		Event = MOUSE_MiddleButtonDown;
		break;
	case WM_LBUTTONUP:
		m_MouseMap.bLButtonDown = false;
		Event = MOUSE_LeftButtonUp;
		break;
	case WM_RBUTTONUP:
		m_MouseMap.bRButtonDown = false;
		Event = MOUSE_RightButtonUp;
		break;
	case WM_MBUTTONUP:
		m_MouseMap.bMButtonDown = false;
		Event = MOUSE_MiddleButtonUp;
		break;
	case WM_MOUSEWHEEL:
		Param.delta = HIWORD(wParam);
		Event = MOUSE_Wheel;
		break;
	case WM_MOUSEMOVE:
		Event = MOUSE_Move;
		break;
	default:
		return;
	}
	m_pWorld->InputMouse(Event, Param);
	m_pViewport->InputMouse(Event, Param);
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
	EKey_Event Event;
	switch(Message)
	{
	case WM_KEYDOWN:
		GKeyMap[Param.Key] = 0x01;
		Event = KEY_Down;
		break;
	case WM_KEYUP:
		GKeyMap[Param.Key] = 0x00;
		Event = KEY_Up;
		break;
	}
	m_pWorld->InputKey(Event, Param);
	m_pViewport->InputKey(Event, Param);
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
		if(StaticThis)
			StaticThis->MessageTranslator(Message, wParam, lParam);
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return 0;
}