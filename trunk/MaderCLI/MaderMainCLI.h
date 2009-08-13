#pragma once

#include "CMaderApp.h"
#include "..\AA\Src\CDirectXDriver.h"
#include "..\AA\Src\BRenderer.h"

#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Interop;

ref class MaderMainCLI : Mader::IMaderMainInterface
{
public:
	Mader::MaderMain^ MainWindow;
	CMaderApp* m_Application;

	virtual void Test()
	{		
		MainWindow->ShowDialog();
	}

	virtual IntPtr GetBackBuffer()
	{
		CDirectXTextureBuffer* pDXTexture = dynamic_cast<CDirectXTextureBuffer*>(m_Application->m_pRenderer->m_pTexture);
		if(!pDXTexture)
			return (IntPtr)NULL;

		D3DSURFACE_DESC Desc;
		pDXTexture->pTexture->GetLevelDesc(0, &Desc);
		CDirectXTextureBuffer* TB = new CDirectXTextureBuffer();
		((CDirectXDriver*)GDriver)->m_pDevice->CreateTexture(Desc.Width, Desc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TB->pTexture, NULL );
		RECT Rect;
		Rect.left = Rect.top = 0;
		Rect.bottom = Desc.Height;
		Rect.right = Desc.Width;
		D3DLOCKED_RECT LRect;
		TB->pTexture->LockRect(0, &LRect, &Rect, D3DLOCK_DISCARD );
		char* Bits = (char*)LRect.pBits;
		for(int i=0;i<Desc.Height;++i)
		{
			for(int j=0;j<LRect.Pitch;++j)
				Bits[i*LRect.Pitch+j] = 255;
		}
		IDirect3DSurface9* Surface;
		TB->pTexture->GetSurfaceLevel(0, &Surface);
		return (IntPtr)Surface;
	}

	virtual bool CreateMaderApp()
	{
		m_Application->WindowInfo.m_hWnd = (HWND)MainWindow->GetHwnd().ToPointer();
		m_Application->WindowInfo.m_hInstance = (HINSTANCE)GetWindowLong(m_Application->WindowInfo.m_hWnd, GWL_HINSTANCE);
		m_Application->WindowInfo.m_wHeight = 300;
		m_Application->WindowInfo.m_wWidth = 300;
		MainWindow->SetHwndSourceHook();
		m_Application->CreateMaderApp();
		return true;
	}

	virtual IntPtr HandleMessages(IntPtr hwnd, int msg, IntPtr wParam,IntPtr lParam,bool% handled)
	{
		int RetVal = 0;

		switch(msg)
		{
		case WM_DESTROY:
		case WM_QUIT:
			m_Application->bQuit = true;
			break;
		}

		return (IntPtr)RetVal;
	}

	MaderMainCLI(CMaderApp* App)
		: m_Application(App)
	{
		MainWindow = gcnew Mader::MaderMain();
		MainWindow->Initialize(this);
	}

	~MaderMainCLI()
	{
		delete MainWindow;
	}
};
