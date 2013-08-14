#pragma once

#include "TDataTypes.h"
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

	TString ToTString(String^ Str);

	virtual void Tick(float deltaTime);
	virtual void Run();

	virtual IntPtr LoadObject(IntPtr World, String^ Path);
	virtual IntPtr LoadAsset(String^ Path);

	virtual IntPtr CreateWorld(String^ Name);
	virtual void DestroyWorld(IntPtr World);

	virtual IntPtr CreateViewport(IntPtr World, Moses::EViewportType ViewportType);
	virtual IntPtr CreateViewportWindow(IntPtr Viewport, IntPtr hWndParent);
	virtual void RemoveViewport(IntPtr World, IntPtr Handle);
	virtual void OnViewportsResized(IntPtr World);

	virtual void SetMousePosition(double X, double Y);
	virtual void MessageTranslator(IntPtr Handle, Moses::Message msg, ... array<System::Object^>^ args);

	virtual void AddToWorld(IntPtr World, IntPtr Thing, IntPtr Asset);
	virtual IntPtr CreateThing(String^ PrimitiveClassName, String^ ThingClassName);


	virtual cli::array<String^>^ CreatablePrimitiveClassNames(IntPtr Asset);
	virtual cli::array<String^>^ CreatableThingClassNames(IntPtr Asset);
	virtual cli::array<String^>^ GetNeededAssetClassNames(IntPtr Asset);

	MosesMainCLI(DMosesApp* App);
	~MosesMainCLI();
};
