#pragma once

#include "CDirectXDriver.h"
#include "BRenderer.h"

#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Interop;

ref class MosesMainCLI : Moses::IMosesMainInterface
{
public:
	Moses::MosesMain^ MainWindow;
	class DMosesApp* m_Application;
	Application^ MyApp;

	virtual void Tick(float deltaTime);
	virtual void Run();
	virtual IntPtr CreateMosesWindow(IntPtr hWndParent);
	virtual void MessageTranslator(Moses::Message msg, ... array<System::Object^>^ args);

	MosesMainCLI(DMosesApp* App);
	~MosesMainCLI();
};
