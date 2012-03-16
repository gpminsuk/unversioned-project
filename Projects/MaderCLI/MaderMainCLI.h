#pragma once

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
	class DMaderApp* m_Application;
	Application^ MyApp;

	virtual IntPtr GetBackBuffer();
	virtual bool CreateMaderApp();
	virtual IntPtr HandleMessages(IntPtr hwnd, int msg, IntPtr wParam,IntPtr lParam,bool% handled);
	virtual void Tick();
	virtual void Run();

	MaderMainCLI(DMaderApp* App);
	~MaderMainCLI();
};
