#include "stdafx.h"

#include <windows.h>
#include "RResource.h"
#include "RMesh.h"
#include "BViewport.h"
#include "BRenderingBatch.h"

#define generic GENERIC
#include "UWorld.h"
#undef GENERIC

#include "DMosesApp.h"
#include "MosesMainCLI.h"
#include "UXMLParser.h"
#include "InputDefine.h"

#include "CSkeletalMeshPrimitive.h"

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

IntPtr MosesMainCLI::LoadObject(IntPtr pWorld, String^ Path)
{
	size_t i;
	pin_ptr<const wchar_t> wch = PtrToStringChars(Path);
	size_t size = ((Path->Length + 1) * 2);
	TString Str;
	wcstombs_s(&i, Str.Str, 1024, wch, Path->Length);
	if(Path->EndsWith(".exskn")) {
		RMesh* Object = LoadAsset<RMesh>(Str);
		UWorld* World = (UWorld*)pWorld.ToPointer();
		CSkeletalMeshPrimitive* pPrimitive = new CSkeletalMeshPrimitive();
		pPrimitive->SetSkeletalMesh(Object, 0);
		pPrimitive->CreateDraws();
		World->m_pRenderer->Render(World->BatchManager, pPrimitive);
	}
	else if(Path->EndsWith(".exmap")) {
		UWorld* World = LoadAsset<UWorld>(Str);
		World->m_pRenderer = m_Application->m_pRenderer;
		World->BatchManager->m_Viewports = &World->Viewports;
		m_Application->m_pRenderer->BatchManager.AddItem(World->BatchManager);
		m_Application->Worlds.AddItem(World);
		for(unsigned int i=0;i<((TWorldOctree*)World->m_pWorldData)->AllObjects.Size();++i) {
			World->m_pRenderer->Render(World->BatchManager, ((TWorldOctree*)World->m_pWorldData)->AllObjects(i));
		}
		return IntPtr(World);
	}
	return IntPtr(0);
}

IntPtr MosesMainCLI::CreateWorld(String^ Name) {
	size_t i;
	pin_ptr<const wchar_t> wch = PtrToStringChars(Name);
	size_t size = ((Name->Length + 1) * 2);
	TString Str;
	wcstombs_s(&i, Str.Str, 1024, wch, Name->Length);
	UWorld* World = (UWorld*)ConstructClass<UWorld*>(Str);
	World->m_pRenderer = m_Application->m_pRenderer;
	World->BatchManager->m_Viewports = &World->Viewports;
	m_Application->m_pRenderer->BatchManager.AddItem(World->BatchManager);
	m_Application->Worlds.AddItem(World);
	return IntPtr(World);
}

void MosesMainCLI::DestroyWorld(IntPtr World) {
	m_Application->Worlds.DeleteItemByVal((UWorld*)World.ToPointer());
	m_Application->m_pRenderer->BatchManager.DeleteItemByVal(((UWorld*)World.ToPointer())->BatchManager);
	delete World.ToPointer();
}

IntPtr MosesMainCLI::CreateViewport(IntPtr World, Moses::EViewportType ViewportType) {
	TViewportInfo Info;
	switch(ViewportType) {
	case Moses::EViewportType::Perspective:
		Info.ProjectionType = Projection_Perpective;
		Info.RenderMode = RenderMode_All;
		Info.CameraMode = CameraMode_Editor;
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
	return IntPtr(((UWorld*)World.ToPointer())->CreateViewport(Info));
}

IntPtr MosesMainCLI::CreateViewportWindow(IntPtr Viewport, IntPtr hWndParent)
{
	return IntPtr(m_Application->CreateViewportWindow((BViewport*)Viewport.ToPointer(), 800, 600, (HWND)hWndParent.ToPointer()));
}

void MosesMainCLI::RemoveViewport(IntPtr World, IntPtr Handle) {
	((UWorld*)World.ToPointer())->RemoveViewport(m_Application->FindViewport((HWND)Handle.ToPointer()));
}

void MosesMainCLI::OnViewportsResized(IntPtr World) {
	((UWorld*)World.ToPointer())->OnViewportsResized();
}

void MosesMainCLI::Run()
{
	MyApp = gcnew Application();
	MyApp->Run(MainWindow);
}

void MosesMainCLI::WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, bool% handled) {
	m_Application->MessageTranslator((HWND)hwnd.ToPointer(), msg, (LPARAM)wParam.ToInt64(), (LPARAM)lParam.ToInt64());
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
	case Moses::Message::MosesMsg_KeyDown:
		{
			KeyEventArgs^ EventArgs = (KeyEventArgs^)args[0];
			TKeyInput_Param Param;
			Param.Key = KeyInterop::VirtualKeyFromKey(EventArgs->Key);
			Param.bLAltDown = Keyboard::IsKeyDown(Key::LeftAlt);
			Param.bLCtrlDown = Keyboard::IsKeyDown(Key::LeftCtrl);
			Param.bLShiftDown = Keyboard::IsKeyDown(Key::LeftShift);
			Param.bRAltDown = Keyboard::IsKeyDown(Key::RightAlt);
			Param.bRCtrlDown = Keyboard::IsKeyDown(Key::RightCtrl);
			Param.bRShiftDown = Keyboard::IsKeyDown(Key::RightShift);
			m_Application->InputKey(m_Application->FindViewport((HWND)Handle.ToPointer()), KEY_Down, Param);
		}
		break;
	case Moses::Message::MosesMsg_KeyUp:
		{
			KeyEventArgs^ EventArgs = (KeyEventArgs^)args[0];
			TKeyInput_Param Param;
			Param.Key = KeyInterop::VirtualKeyFromKey(EventArgs->Key);
			Param.bLAltDown = Keyboard::IsKeyDown(Key::LeftAlt);
			Param.bLCtrlDown = Keyboard::IsKeyDown(Key::LeftCtrl);
			Param.bLShiftDown = Keyboard::IsKeyDown(Key::LeftShift);
			Param.bRAltDown = Keyboard::IsKeyDown(Key::RightAlt);
			Param.bRCtrlDown = Keyboard::IsKeyDown(Key::RightCtrl);
			Param.bRShiftDown = Keyboard::IsKeyDown(Key::RightShift);
			m_Application->InputKey(m_Application->FindViewport((HWND)Handle.ToPointer()), KEY_Up, Param);
		}
		break;
	}
}