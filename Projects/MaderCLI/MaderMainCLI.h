#pragma once

#include "DMaderApp.h"
#include "CDirectXDriver.h"
#include "BRenderer.h"

#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Interop;

ref class MaderMainCLI : Mader::IMaderMainInterface
{
public:
	Mader::MaderMain^ MainWindow;
	DMaderApp* m_Application;

	virtual IntPtr GetBackBuffer()
	{		
		return (IntPtr)0;
	}

	virtual bool CreateMaderApp()
	{
		m_Application->m_WindowInfo.m_hWnd = (HWND)MainWindow->GetHwnd().ToPointer();
#if _WIN64
		m_Application->m_WindowInfo.m_hInstance = (HINSTANCE)GetWindowLong(m_Application->m_WindowInfo.m_hWnd, GWLP_HINSTANCE);
#else
		m_Application->m_WindowInfo.m_hInstance = (HINSTANCE)GetWindowLong(m_Application->m_WindowInfo.m_hWnd, GWL_HINSTANCE);
#endif
		m_Application->m_WindowInfo.m_wHeight = 600;
		m_Application->m_WindowInfo.m_wWidth = 800;
		MainWindow->SetHwndSourceHook();
		m_Application->CreateApp(m_Application->m_WindowInfo);
		return true;
	}

	virtual IntPtr HandleMessages(IntPtr hwnd, int msg, IntPtr wParam,IntPtr lParam,bool% handled)
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

	MaderMainCLI(DMaderApp* App)
		: m_Application(App)
	{
		MainWindow = gcnew Mader::MaderMain();
		MainWindow->Initialize(this);
		
		Application^ MyApp = gcnew Application();
		MyApp->Run(MainWindow);
	}

	~MaderMainCLI()
	{
		delete MainWindow;
	}

	virtual void Tick()
	{
		m_Application->Tick(0);
		m_Application->m_pRenderer->ThreadExecute();
	}
};
