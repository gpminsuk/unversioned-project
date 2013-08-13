#pragma once

#include "CDirectXDriver.h"
#include "BRenderer.h"

#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Interop;
using namespace System::Windows::Input;

ref class MosesMainCLI : Moses::IMosesMainInterface
{
public:
	Moses::MosesMain^ MainWindow;
	class DMosesApp* m_Application;
	Application^ MyApp;

	virtual void Tick(float deltaTime);
	virtual void Run();

	virtual IntPtr LoadObject(IntPtr World, String^ Path);

	virtual IntPtr CreateWorld(String^ Name);
	virtual void DestroyWorld(IntPtr World);

	virtual IntPtr CreateViewport(IntPtr World, Moses::EViewportType ViewportType);
	virtual IntPtr CreateViewportWindow(IntPtr Viewport, IntPtr hWndParent);
	virtual void RemoveViewport(IntPtr World, IntPtr Handle);
	virtual void OnViewportsResized(IntPtr World);

	virtual void WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, bool% handled);
	virtual void MessageTranslator(IntPtr Handle, Moses::Message msg, ... array<System::Object^>^ args);

	MosesMainCLI(DMosesApp* App);
	~MosesMainCLI();
};
