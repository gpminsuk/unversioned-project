#include "stdafx.h"

#include <windows.h>
#include "DMosesApp.h"
#include "MosesMainCLI.h"
#include "UXMLParser.h"
#include "InputDefine.h"

using namespace System::Windows::Input;

INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	CXMLApplicationParser Parser;
	Parser.ReadXMLFile("..\\..\\Config\\Tool.config");
	Parser.Parse();
}

MosesMainCLI::MosesMainCLI(DMosesApp* App)
: m_Application(App)
{
	MainWindow = gcnew Moses::MosesMain(this);
}

MosesMainCLI::~MosesMainCLI()
{
	delete MainWindow;
}

void MosesMainCLI::Tick(float deltaTime)
{
	m_Application->Tick(0);
	m_Application->m_pRenderer->ThreadExecute();
}

IntPtr MosesMainCLI::CreateMosesWindow(IntPtr hWndParent)
{
	return IntPtr(m_Application->CreateMosesRenderView((HWND)hWndParent.ToPointer()));
}

void MosesMainCLI::Run()
{
	MyApp = gcnew Application();
	MyApp->Run(MainWindow);
}

void MosesMainCLI::MessageTranslator(Moses::Message msg, ... array<System::Object^>^ args)
{
	switch(msg)
	{
	case Moses::Message::MosesMsg_MouseMove:
		{
			MouseEventArgs^ EventArgs = (MouseEventArgs^)args[0];
			TMouseInput_Param Param;
			Param.bLButtonDown = (unsigned char)EventArgs->LeftButton;
			Param.bRButtonDown = (unsigned char)EventArgs->RightButton;
			Param.bMButtonDown = (unsigned char)EventArgs->MiddleButton;
			Point pt = EventArgs->GetPosition(nullptr);
			Param.X = (unsigned short)pt.X;
			Param.Y = (unsigned short)pt.Y;
			if(m_Application->m_MousePt.x) 
				Param.dX = m_Application->m_MousePt.x - Param.X;
			else
				Param.dX = 0;
			if(m_Application->m_MousePt.y)
				Param.dY = m_Application->m_MousePt.y - Param.Y;
			else
				Param.dY = 0;
			if(m_Application->m_MousePt.x && m_Application->m_MousePt.y && Param.dX == 0 && Param.dY == 0)
				return;
			m_Application->m_MousePt.x = Param.X;
			m_Application->m_MousePt.y = Param.Y;
			m_Application->InputMouse(MOUSE_Move, Param);
		}
		break;
	}
}