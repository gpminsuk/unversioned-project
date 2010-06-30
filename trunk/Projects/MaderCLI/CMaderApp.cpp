#include "StdAfx.h"
#include "CMaderApp.h"
#include "..\Unique\Src\BRenderer.h"
#include "..\Unique\Src\BViewport.h"
#include "..\Unique\Src\CDirectXDriver.h"
#include "..\Unique\Src\World\CCameraViewport.h"
#include "..\Unique\Src\RResourceManager.h"

#define generic GENERIC
#include "..\Unique\Src\World\UWorld.h"
#undef GENERIC

CMaderApp::CMaderApp()
{

}

bool CMaderApp::CreateApp(TApplicationInfo& Info)
{
	m_WindowInfo = (TCSharpWindowInfo&)Info;

	TDXWindowInfo DXWindowInfo;
	DXWindowInfo.m_hWnd = m_WindowInfo.m_hWnd;
	DXWindowInfo.m_wHeight = m_WindowInfo.m_wHeight;
	DXWindowInfo.m_wWidth = m_WindowInfo.m_wWidth;

	GDriver = new CDirectXDriver(DXWindowInfo);
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
	return true;
}

void CMaderApp::Do()
{
	MSG msg;

	while(!bQuit)
	{
		Sleep(1000);	
	}
}

void CMaderApp::Tick(unsigned long Time)
{
	if(m_pViewport)
	{
		m_pViewport->UpdateViewport();
	}
	if(m_pWorld)
	{
		m_pWorld->Tick(Time);
	}
}

void CMaderApp::MouseEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam)
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

void CMaderApp::KeyEventTranslator(UINT Message, WPARAM wParam, LPARAM lParam)
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

void CMaderApp::MessageTranslator(UINT Message, WPARAM wParam, LPARAM lParam)
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