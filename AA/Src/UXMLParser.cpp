#include "stdafx.h"
#include "UXMLParser.h"

#include "BRenderer.h"

#include "CWindowApp.h"
#include "CDirectXDriver.h"

#include "UWorld.h"

UXMLParser::UXMLParser()
{
}

UXMLParser::~UXMLParser()
{
}

void UXMLParser::StartElement(void *UserData, const char *Name, const char **Atts)
{
	UXMLParser* Parser = (UXMLParser*)UserData;
	TXMLElement Element;
	Element.ElementName = Name;
	unsigned int Idx = 0;
	const char *Att = *Atts;
	while(Atts[Idx])
	{
		TXMLAttribute Attribute;
		Attribute.AttributeName = Atts[Idx];
		Idx++;
		if(Atts[Idx])
		{
			Attribute.AttributeValue = Atts[Idx];
			Idx++;
		}
		Element.Attributes.AddItem(Attribute);
	}
	Element.Parent = Parser->ParentElement;
	Parser->ParentElement->ChildElements.AddItem(Element);
	Parser->ParentElement = &Parser->ParentElement->ChildElements.EndItem();			
}

void UXMLParser::EndElement(void *UserData, const char *Name)
{
	UXMLParser* Parser = (UXMLParser*)UserData;
	Parser->ParentElement = Parser->ParentElement->Parent;
}

bool UXMLParser::ReadXMLFile(char* FileName)
{
	FILE* FilePointer = NULL;
	fopen_s(&FilePointer, FileName, "r");
	if(FilePointer)
	{
		Parser = XML_ParserCreate(NULL);
		ParentElement = &Root;
		char buf[BUFSIZ];
		int Done;
		XML_SetUserData(Parser, this);
		XML_SetElementHandler(Parser, UXMLParser::StartElement, UXMLParser::EndElement);
		do
		{
			int len = (int)fread(buf, 1, sizeof(buf), FilePointer);
			Done = len < sizeof(buf);
			if (!XML_Parse(Parser, buf, len, Done))
			{
				printf("%s at line %d\n", XML_ErrorString(XML_GetErrorCode(Parser)), XML_GetCurrentLineNumber(Parser));
				return false;
			}
		} while (!Done);
		fclose(FilePointer);
		XML_ParserFree(Parser);
		return true;
	}	
	return false;
}

bool UXMLParser::GetValue(char* Path, TString& Ret)
{
	return Root.GetValue(Path, Ret);
}

/////////////////////////////////// Application Config Parser ///////////////////////////////////

UXMLApplicationParser::UXMLApplicationParser()
{

}

UXMLApplicationParser::~UXMLApplicationParser()
{

}

void UXMLApplicationParser::Parse()
{	
	TXMLElement Application;
	if(Root.GetChildElement("Application", Application))
	{
		BApplication *app = 0;
		if(Application.HasAttribute("Platform","Windows_x86"))
		{
			TWindowsInfo Info;
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
						GDriver = new CDirectXDriver(&WindowsApp->m_WindowInfo);
						GDriver->CreateDriver();
					}
				}			
			}
			TString Value;
			app->m_pRenderer = new BRenderer(app);
		}
		TXMLElement World;
		if(Application.GetChildElement("World", World))
		{
			app->m_pWorld = new UWorld(app->m_pRenderer);
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