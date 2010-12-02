#include "stdafx.h"
#include "MyXMLParser.h"

#include "BRenderer.h"

#include "CWindowApp.h"
#include "CCameraViewport.h"
#include "CDirectXDriver.h"
#include "CWaveIODriver.h"

#include "MyWorld.h"

MyXMLParser::MyXMLParser()
{

}

MyXMLParser::~MyXMLParser()
{

}

void MyXMLParser::Parse()
{
	TXMLElement Application;
	if(Root.GetChildElement("Application", Application))
	{
		AApplication *app = 0;
		if(Application.HasAttribute("Platform","Windows_x86"))
		{
			TWindowInfo Info;
			TString Value;
			if(Application.GetValue("Resolution.Width", Value))
			{
				Info.m_wWidth = Value.ToInt();
			}
			if(Application.GetValue("Resolution.Height", Value))
			{
				Info.m_wHeight = Value.ToInt();
			}
			app = new CWindowApp();
			if(!app->CreateApp(Info))
			{
				app->DestroyApp();
				app = 0;
			}
		}
		TXMLElement Renderer;
		if(Application.GetChildElement("Renderer", Renderer))
		{
			TRendererInfo Info;
			TXMLElement Driver;
			if(Renderer.GetChildElement("Driver", Driver))
			{
				if(Driver.HasAttribute("Platform", "DirectX"))
				{
					CWindowApp* WindowsApp = static_cast<CWindowApp*>(app);
					if(WindowsApp)
					{
						TWindowInfo& WindowInfo = (TWindowInfo&)WindowsApp->m_WindowInfo;
						TDXWindowInfo DXWindowInfo;
						DXWindowInfo.m_hWnd = WindowInfo.m_hWnd;
						DXWindowInfo.m_wHeight = WindowInfo.m_wHeight;
						DXWindowInfo.m_wWidth = WindowInfo.m_wWidth;
						GDriver = new CDirectXDriver(DXWindowInfo);
						GDriver->CreateDriver();
						GSoundDriver = new CWaveIODriver();
						//TString FN;
						//FN = "..\\..\\Resources\\tada.wav";
						//GSoundDriver->LoadSound(FN);
						//GSoundDriver->PlayWAVSound();
					}
				}			
			}
			TString Value;
			app->m_pRenderer = new BRenderer(app);
		}
		TXMLElement World;
		if(Application.GetChildElement("World", World))
		{
			app->m_pWorld = new UMyWorld();
			app->m_pViewport = new CCameraViewport();
			app->m_pRenderer->AddViewport(app->m_pViewport);
			app->m_pWorld->AddViewport(app->m_pViewport);
		}

		if(app)
		{
			app->Initialize();
			app->Do();
			app->DestroyApp();
			delete app;
		}
	}
}