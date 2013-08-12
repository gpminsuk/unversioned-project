#include "stdafx.h"

#include <windows.h>
#include "BViewport.h"

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

IntPtr MosesMainCLI::CreateViewport(Moses::EViewportType ViewportType) {
	TViewportInfo Info;
	switch(ViewportType) {
	case Moses::EViewportType::Perspective:
		Info.ProjectionType = Projection_Perpective;
		Info.RenderMode = RenderMode_All;
		Info.CameraMode = CameraMode_FirstPerson;
		break;
	case Moses::EViewportType::Top:
		Info.ProjectionType = Projection_Orthogonal;
		Info.RenderMode = RenderMode_Wireframe;
		Info.CameraMode = CameraMode_Top;
		break;
	case Moses::EViewportType::Back:
		Info.ProjectionType = Projection_Orthogonal;
		Info.RenderMode = RenderMode_Wireframe;
		Info.CameraMode = CameraMode_Back;
		break;
	case Moses::EViewportType::Bottom:
		Info.ProjectionType = Projection_Orthogonal;
		Info.RenderMode = RenderMode_Wireframe;
		Info.CameraMode = CameraMode_Bottom;
		break;
	case Moses::EViewportType::Front:
		Info.ProjectionType = Projection_Orthogonal;
		Info.RenderMode = RenderMode_Wireframe;
		Info.CameraMode = CameraMode_Front;
		break;
	case Moses::EViewportType::Left:
		Info.ProjectionType = Projection_Orthogonal;
		Info.RenderMode = RenderMode_Wireframe;
		Info.CameraMode = CameraMode_Left;
		break;
	case Moses::EViewportType::Right:
		Info.ProjectionType = Projection_Orthogonal;
		Info.RenderMode = RenderMode_Wireframe;
		Info.CameraMode = CameraMode_Right;
		break;
	}
	return IntPtr(m_Application->CreateViewport(Info));
}

IntPtr MosesMainCLI::CreateViewportWindow(IntPtr Viewport, IntPtr hWndParent)
{
	return IntPtr(m_Application->CreateViewportWindow((BViewport*)Viewport.ToPointer(), 800, 600, (HWND)hWndParent.ToPointer()));
}

void MosesMainCLI::RemoveViewport(IntPtr Handle) {
	m_Application->RemoveViewport(m_Application->FindViewport((HWND)Handle.ToPointer()));
}

void MosesMainCLI::OnViewportsResized() {
	m_Application->OnViewportsResized();
}

void MosesMainCLI::Run()
{
	MyApp = gcnew Application();
	MyApp->Run(MainWindow);
}

void MosesMainCLI::MessageTranslator(IntPtr Handle, Moses::Message msg, ... array<System::Object^>^ args)
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
			Param.dX = m_Application->m_MousePt.x - Param.X;
			Param.dY = m_Application->m_MousePt.y - Param.Y;
			m_Application->m_MousePt.x = Param.X;
			m_Application->m_MousePt.y = Param.Y;
			m_Application->InputMouse(m_Application->FindViewport((HWND)Handle.ToPointer()), MOUSE_Move, Param);
		}
		break;
	case Moses::Message::MosesMsg_MouseWheel:
		{
			MouseWheelEventArgs^ EventArgs = (MouseWheelEventArgs^)args[0];
			TMouseInput_Param Param;
			Param.bLButtonDown = (unsigned char)EventArgs->LeftButton;
			Param.bRButtonDown = (unsigned char)EventArgs->RightButton;
			Param.bMButtonDown = (unsigned char)EventArgs->MiddleButton;
			Param.delta = -EventArgs->Delta;
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
			m_Application->m_MousePt.x = Param.X;
			m_Application->m_MousePt.y = Param.Y;
			m_Application->InputMouse(m_Application->FindViewport((HWND)Handle.ToPointer()), MOUSE_Wheel, Param);
		}
		break;
	}
}