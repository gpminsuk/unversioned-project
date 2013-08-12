#include "StdAfx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "BDriver.h"
#include "BRenderPass.h"

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

void CWindowsApplication::Initialize() {
	GDriver->CreateDriver(this);
	m_pRenderer = new BRenderer(this);
	m_pWorld->m_pRenderer = m_pRenderer;
	m_pRenderer->m_Viewports = Viewports;
	GRenderPassResource.Initialize(m_WindowInfo.m_wWidth, m_WindowInfo.m_wHeight);
    RResourceManager::LoadResources();
    bRenderThreadQuit = false;
    m_pRenderer->Start();
    
	if (m_pWorld) {
        m_pWorld->InitializeWorld();
    }
}

BViewport* CWindowsApplication::FindViewport(HWND hWnd) {
	for(unsigned int i=0;i<Viewports.Size();++i) {
		CWindowsViewport* Viewport = dynamic_cast<CWindowsViewport*>(Viewports(i));
		if(Viewport->Handle == hWnd) {
			return Viewports(i);
		}
	}
	return Viewports(0);
}

BViewport* CWindowsApplication::CreateViewport(TViewportInfo& Info) {
	BViewport* Viewport = new CWindowsViewport(Info.m_wWidth, Info.m_wHeight, Info.ProjectionType, Info.RenderMode, Info.CameraMode, Info.m_hWnd);
	Viewports.AddItem(Viewport);
	return Viewport;
}

void CWindowsApplication::RemoveViewport(BViewport* Viewport) {
	Viewports.DeleteItemByVal(Viewport);
}

bool CWindowsApplication::CreateApp(TApplicationInfo& Info) {
    m_WindowInfo = (TWindowInfo&) Info;
	TViewportInfo ViewportInfo;
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

	ViewportInfo.m_hWnd = ::CreateWindow(
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
	ViewportInfo.m_wWidth = m_WindowInfo.m_wWidth;
	ViewportInfo.m_wHeight = m_WindowInfo.m_wHeight;
    ShowWindow(ViewportInfo.m_hWnd, SW_SHOW);
    UpdateWindow(ViewportInfo.m_hWnd);

	m_WindowInfo.m_hWnd = ViewportInfo.m_hWnd;
	Viewports.AddItem(new CWindowsViewport(m_WindowInfo.m_wWidth, m_WindowInfo.m_wHeight, Projection_Perpective, RenderMode_All, CameraMode_FreeMode, ViewportInfo.m_hWnd));
	return true;
}

bool CWindowsApplication::DestroyApp() {
    if (m_pWorld)
        m_pWorld->DestroyWorld();
    delete m_pWorld;
    m_pWorld = 0;

    delete m_pRenderer;
    m_pRenderer = 0;

	for(unsigned int i=0;i<Viewports.Size();++i) {
		delete Viewports(i);
	}
	Viewports.Clear();
   

    RResourceManager::ReleaseAllResources();
    return true;
}

void CWindowsApplication::Do() {
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
	for(unsigned int i=0;i<Viewports.Size();++i) {
		BViewport* Viewport = Viewports(i);
		Viewport->UpdateViewport();
	}
    m_pWorld->Tick(Time);
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
        GKeyMap[Param.Key] = 0x01;
        Event = KEY_Down;
        break;
    case WM_KEYUP:
        GKeyMap[Param.Key] = 0x00;
        Event = KEY_Up;
        break;
    }
    InputKey(Viewport, Event, Param);
}

void CWindowsApplication::OnViewportsResized() {
	unsigned int MaxWidth = 0;
	unsigned int MaxHeight = 0;
	for(unsigned int i=0;i<Viewports.Size();++i) {
		Viewports(i)->OnSizeChanged();
		if(MaxWidth < Viewports(i)->Width) {
			MaxWidth = Viewports(i)->Width;
		}
		if(MaxHeight < Viewports(i)->Height) {
			MaxHeight = Viewports(i)->Height;
		}
	}
	if(m_pRenderer) {
		GRenderPassResource.Initialize(MaxWidth, MaxHeight);
	}
}

void CWindowsApplication::InputMouse(BViewport* Viewport, EMouse_Event Event, TMouseInput_Param& Param) {
    m_pWorld->InputMouse(Event, Param);
    Viewport->InputMouse(Event, Param);
}

void CWindowsApplication::InputKey(BViewport* Viewport, EKey_Event Event, TKeyInput_Param& Param) {
    m_pWorld->InputKey(Event, Param);
	Viewport->InputKey(Event, Param);
}

void CWindowsApplication::MessageTranslator(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_SIZE:
		OnViewportsResized();
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
