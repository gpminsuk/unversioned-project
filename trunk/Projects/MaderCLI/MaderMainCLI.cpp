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
	MainWindow = gcnew Mader::MaderMain(this);
}

MaderMainCLI::~MaderMainCLI()
{
	delete MainWindow;
}

void MaderMainCLI::Tick(float deltaTime)
{
	m_Application->Tick(0);
	m_Application->m_pRenderer->ThreadExecute();
}

IntPtr MaderMainCLI::CreateMaderWindow(IntPtr hWndParent)
{
	return IntPtr(m_Application->CreateMaderRenderView((HWND)hWndParent.ToPointer()));
}

void MaderMainCLI::Run()
{
	MyApp = gcnew Application();
	MyApp->Run(MainWindow);
}