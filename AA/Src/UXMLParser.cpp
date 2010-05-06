#include "stdafx.h"
#include "UXMLParser.h"
#include "CWindowApp.h"

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
	if(Parser->ParentElement)
	{
		Parser->ParentElement->ChildElements.AddItem(Element);
		Parser->ParentElement = &Parser->ParentElement->ChildElements.EndItem();		
	}
	else
	{
		Parser->Elements.AddItem(Element);
		Parser->ParentElement = &Parser->Elements.EndItem();
	}	
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
		ParentElement = NULL;
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

/////////////////////////////////// Application Config Parser ///////////////////////////////////

UXMLApplicationParser::UXMLApplicationParser()
{

}

UXMLApplicationParser::~UXMLApplicationParser()
{

}

void UXMLApplicationParser::Parse()
{
	for(unsigned int i=0;i<Elements.Size();++i)
	{
		TXMLElement& Element = Elements(i);
		if(Element.IsElementName("Application") && Element.HasAttribute("Platform","Windows_x86"))
		{
			TWindowsInfo Info;
			TXMLElement& ApplicationElement = Element;
			if(Element.GetChildElement("Resolution", ApplicationElement))
			{
				TString Value;
				Element.GetAttributeValue("Width", Value);
				Info.m_wWidth = Value.ToInt();
				Element.GetAttributeValue("Height", Value);
				Info.m_wHeight = Value.ToInt();
			}
			BApplication *app = new CWindowApp();
			if(app->CreateApp(Info))
				app->Do();
			app->DestroyApp();
			delete app;
		}
	}	
}