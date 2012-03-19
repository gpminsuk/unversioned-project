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

	virtual void Tick(float deltaTime);
	virtual void Run();
	virtual IntPtr CreateMaderWindow(IntPtr hWndParent);

	MaderMainCLI(DMaderApp* App);
	~MaderMainCLI();
};
