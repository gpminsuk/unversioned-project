#include "StdAfx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "BDriver.h"
#include "BRenderPass.h"
#include "BRenderingBatch.h"

#include "InputDefine.h"

#include "UWorld.h"

#include "CWindowsApplication.h"
#include "CWindowsViewport.h"

#include "RResourceManager.h"

CWindowsApplication* CWindowsApplication::StaticThis = 0;
SYSTEM_INFO GSystemInformation;
unsigned char GKeyMap[KEYMAP_SIZE] = { 0, };

CWindowsApplication::CWindowsApplication() {
    StaticThis = this;

    m_MouseMap.bLButtonDown = 0;
    m_MouseMap.bRButtonDown = 0;
    m_MouseMap.bMButtonDown = 0;

    GetSystemInfo(&GSystemInformation);

	m_pRenderer = 0;
}

BViewport* CWindowsApplication::FindViewport(HWND hWnd) {
	for(unsigned int i=0;i<Worlds.Size();++i) {
		BViewport* Viewport = Worlds(i)->FindViewport(hWnd);
		if(Viewport) {
			return Viewport;
		}
	}
	return 0;
}

bool CWindowsApplication::CreateApplicationWindow(TApplicationInfo& Info) {
    m_WindowInfo = (TWindowInfo&) Info;
    m_WindowInfo.m_hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_WindowInfo.m_hInstance;
    wcex.hIcon = LoadIcon(m_WindowInfo.m_hInstance, 0);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = _T("CLASS NAME");
    wcex.hIconSm = LoadIcon(m_WindowInfo.m_hInstance, 0);

    RegisterClassEx(&wcex);

	m_WindowInfo.m_hWnd = ::CreateWindow(
		_T("CLASS NAME"),
        _T("CAPTION"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_WindowInfo.m_wWidth + 14,
		m_WindowInfo.m_wHeight + 36, 
		NULL,
		0,
		m_WindowInfo.m_hInstance,
		0);

    ShowWindow(m_WindowInfo.m_hWnd, SW_SHOW);
    UpdateWindow(m_WindowInfo.m_hWnd);

	GDriver->CreateDriver(this);
	m_pRenderer = new BRenderer(this);

	RResourceManager::LoadResources();
	GRenderPassResource.Initialize(m_WindowInfo.m_wWidth, m_WindowInfo.m_wHeight);

	bRenderThreadQuit = false;
	m_pRenderer->Start();
	return true;
}

bool CWindowsApplication::DestroyApp() {
    delete m_pRenderer;
    m_pRenderer = 0;
		
    RResourceManager::ReleaseAllResources();
    return true;
}

void CWindowsApplication::Do() {
	static bool initialized = false;
	if(!initialized) {
		Worlds(0)->Viewports.AddItem(new CWindowsViewport(m_WindowInfo.m_wWidth, m_WindowInfo.m_wHeight, Projection_Perpective, RenderMode_All, CameraMode_FreeMode, m_WindowInfo.m_hWnd));
		initialized = true;
	}
    MSG msg;
    int Count = 0;
    DWORD PrevTime = timeGetTime();
    while (!bQuit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT || GKeyMap[VK_ESCAPE]) {
                bQuit = true;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            if ((timeGetTime() - PrevTime) > 16) {
                Tick(timeGetTime() - PrevTime);
                PrevTime = timeGetTime();
                m_pRenderer->ThreadExecute();
            }
        }
        //Sleep(1);
    }
    //while(!bRenderThreadQuit);
}

bool CWindowsApplication::Tick(unsigned long Time) {	
	for(unsigned int i=0;i<Worlds.Size();++i) {
		UWorld* World = Worlds(i);
		World->Tick(Time);
	}
    return true;
}

void CWindowsApplication::SetMousePos(float X, float Y, bool isRatio) {
    POINT MousePt;
    if (isRatio) {
        RECT rt;
        ::GetClientRect(m_WindowInfo.m_hWnd, &rt);
        MousePt.x = (LONG)(X * (rt.right - rt.left));
        MousePt.y = (LONG)(Y * (rt.bottom - rt.top));
    } else {
        MousePt.x = (LONG) X;
        MousePt.y = (LONG) Y;
    }
    m_MousePt.x = MousePt.x;
    m_MousePt.y = MousePt.y;
    ::ClientToScreen(m_WindowInfo.m_hWnd, &MousePt);
    ::SetCursorPos((int) MousePt.x, (int) MousePt.y);
}

void CWindowsApplication::MouseEventTranslator(BViewport* Viewport, UINT Message, WPARAM wParam, LPARAM lParam) {
    TMouseInput_Param Param;
    Param.bLButtonDown = m_MouseMap.bLButtonDown;
    Param.bRButtonDown = m_MouseMap.bRButtonDown;
    Param.bMButtonDown = m_MouseMap.bMButtonDown;
    Param.X = LOWORD(lParam);
    Param.Y = HIWORD(lParam);
    if (m_MousePt.x)
        Param.dX = m_MousePt.x - Param.X;
    else
        Param.dX = 0;
    if (m_MousePt.y)
        Param.dY = m_MousePt.y - Param.Y;
    else
        Param.dY = 0;
    if (m_MousePt.x && m_MousePt.y && Param.dX == 0 && Param.dY == 0)
        return;
    m_MousePt.x = Param.X;
    m_MousePt.y = Param.Y;
    EMouse_Event Event;
    switch (Message) {
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
    InputMouse(Viewport, Event, Param);
}

void CWindowsApplication::KeyEventTranslator(BViewport* Viewport, UINT Message, WPARAM wParam, LPARAM lParam) {
    TKeyInput_Param Param;
    Param.Key = (unsigned short) wParam;
    EKey_Event Event;
    switch (Message) {
	case WM_KEYDOWN:
        Event = KEY_Down;
        break;
    case WM_KEYUP:
        Event = KEY_Up;
        break;
    }
    InputKey(Viewport, Event, Param);
}

void CWindowsApplication::InputMouse(BViewport* Viewport, EMouse_Event Event, TMouseInput_Param& Param) {
	for(unsigned int i=0;i<Worlds.Size();++i) {
		UWorld* World = Worlds(i);
		World->InputMouse(Event, Param);
	}
    Viewport->InputMouse(Event, Param);
}

void CWindowsApplication::InputKey(BViewport* Viewport, EKey_Event Event, TKeyInput_Param& Param) {
	switch(Event) {
	case KEY_Down:
		GKeyMap[Param.Key] = 0x01;
		break;
	case KEY_Up:
		GKeyMap[Param.Key] = 0x00;
		break;
	}
	for(unsigned int i=0;i<Worlds.Size();++i) {
		UWorld* World = Worlds(i);
		World->InputKey(Event, Param);
	}
	Viewport->InputKey(Event, Param);
}

void CWindowsApplication::MessageTranslator(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_SIZE:
		for(unsigned int i=0;i<Worlds.Size();++i) {
			Worlds(i)->OnViewportsResized();
		}
		break;
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
        MouseEventTranslator(FindViewport(Handle), Message, wParam, lParam);
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
        KeyEventTranslator(FindViewport(Handle), Message, wParam, lParam);
        break;
    }
}

LRESULT CALLBACK CWindowsApplication::Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch (Message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        if (StaticThis)
            StaticThis->MessageTranslator(hWnd, Message, wParam, lParam);
        return DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0;
}
