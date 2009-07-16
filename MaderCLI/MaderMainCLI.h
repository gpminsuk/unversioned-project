#pragma once

#include "CMaderApp.h"

#using <System.Windows.Forms.dll>

using namespace System::Windows;

ref class MaderMainCLI : Mader::IMaderMainInterface
{
public:
	Mader::MaderMain^ MainWindow;
	CMaderApp* m_Application;

	virtual void Test()
	{
		MainWindow = gcnew Mader::MaderMain();
		MainWindow->ShowDialog();
	}

	virtual bool CreateMaderApp()
	{
		m_Application->CreateMaderApp();
		return true;
	}

	MaderMainCLI(CMaderApp* App)
		: m_Application(App)
	{
	}

	~MaderMainCLI()
	{
		delete MainWindow;
	}
};
