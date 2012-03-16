#include "stdafx.h"

#include <windows.h>
#include "DMaderApp.h"
#include "MaderMainCLI.h"
#include "UXMLParser.h"

INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	CXMLApplicationParser Parser;
	Parser.ReadXMLFile("..\\..\\Config\\Tool.config");
	Parser.Parse();
}

MaderMainCLI::MaderMainCLI(DMaderApp* App)
: m_Application(App)
{
	MainWindow = gcnew Mader::MaderMain();
	MainWindow->Initialize(this);
}

MaderMainCLI::~MaderMainCLI()
{
	delete MainWindow;
}

void MaderMainCLI::Tick()
{
	m_Application->Tick(0);
	m_Application->m_pRenderer->ThreadExecute();
}

IntPtr MaderMainCLI::GetBackBuffer()
{		
	return (IntPtr)0;
}

bool MaderMainCLI::CreateMaderApp()
{
	m_Application->m_WindowInfo.m_hWnd = (HWND)MainWindow->GetWindowHandle().ToPointer();
	m_Application->m_WindowInfo.m_hRendererWnd = (HWND)MainWindow->GetRendererWindowHandle().ToPointer();
#if _WIN64
	m_Application->m_WindowInfo.m_hInstance = (HINSTANCE)GetWindowLong(m_Application->m_WindowInfo.m_hWnd, GWLP_HINSTANCE);
#else
	m_Application->m_WindowInfo.m_hInstance = (HINSTANCE)GetWindowLong(m_Application->m_WindowInfo.m_hWnd, GWL_HINSTANCE);
#endif	
	MainWindow->GetRendererWindowSize(m_Application->m_WindowInfo.m_wWidth, m_Application->m_WindowInfo.m_wHeight);
	MainWindow->SetHwndSourceHook();
	return true;
}

IntPtr MaderMainCLI::HandleMessages(IntPtr hwnd, int msg, IntPtr wParam,IntPtr lParam,bool% handled)
{
	int RetVal = 0;

	switch(msg)
	{
	case WM_DESTROY:
	case WM_QUIT:
		PostQuitMessage(0);
		m_Application->bQuit = true;
		break;
	default:
		m_Application->MessageTranslator(msg, (DWORD)wParam.ToPointer(), (DWORD)lParam.ToPointer());
	}

	return (IntPtr)RetVal;
}

void MaderMainCLI::Run()
{
	MyApp = gcnew Application();
	MyApp->Run(MainWindow);
}