#include "stdafx.h"
#include "UXMLParser.h"

#include "BRenderer.h"

#include "CWindowsApplication.h"
#include "CDirectXDriver.h"
#include "CWaveIODriver.h"
#include "BViewport.h"

#include "RSKNImporter.h"

#include "UWorld.h"

BXMLParser::BXMLParser() {
}

BXMLParser::~BXMLParser() {
}

void BXMLParser::StartElement(void *UserData, const char *Name, const char **Atts) {
    BXMLParser* Parser = (BXMLParser*) UserData;
    TXMLElement Element;
    Element.ElementName = Name;
    unsigned int Idx = 0;
    const char *Att = *Atts;
    while (Atts[Idx]) {
        TXMLAttribute Attribute;
        Attribute.AttributeName = Atts[Idx];
        Idx++;
        if (Atts[Idx]) {
            Attribute.AttributeValue = Atts[Idx];
            Idx++;
        }
        Element.Attributes.AddItem(Attribute);
    }
    Element.Parent = Parser->ParentElement;
    Parser->ParentElement->ChildElements.AddItem(Element);
    Parser->ParentElement = &Parser->ParentElement->ChildElements.EndItem();
}

void BXMLParser::EndElement(void *UserData, const char *Name) {
    BXMLParser* Parser = (BXMLParser*) UserData;
    Parser->ParentElement = Parser->ParentElement->Parent;
}

bool BXMLParser::ReadXMLFile(char* FileName) {
    FILE* FilePointer = NULL;
    fopen_s(&FilePointer, FileName, "r");
    if (FilePointer) {
        Parser = XML_ParserCreate(NULL);
        ParentElement = &Root;
        char buf[BUFSIZ];
        int Done;
        XML_SetUserData(Parser, this);
        XML_SetElementHandler(Parser, BXMLParser::StartElement,
                              BXMLParser::EndElement);
        do {
            int len = (int) fread(buf, 1, sizeof(buf), FilePointer);
            Done = len < sizeof(buf);
            if (!XML_Parse(Parser, buf, len, Done)) {
                printf("%s at line %d\n",
                       XML_ErrorString(XML_GetErrorCode(Parser)),
                       XML_GetCurrentLineNumber(Parser));
                return false;
            }
        } while (!Done);
        fclose(FilePointer);
        XML_ParserFree(Parser);
        return true;
    }
    return false;
}

bool BXMLParser::GetValue(char* Path, TString& Ret) {
    return Root.GetValue(Path, Ret);
}

/////////////////////////////////// Application Config Parser ///////////////////////////////////

CXMLApplicationParser::CXMLApplicationParser() {

}

CXMLApplicationParser::~CXMLApplicationParser() {

}

void CXMLApplicationParser::Parse() {
    TXMLElement Application;
    if (Root.GetChildElement("Application", Application)) {
		AApplication *app = 0;
		TWindowInfo Info;
        if (Application.HasAttribute("Platform", "Windows_x86")) {
            TString Value;
            if (Application.GetValue("Resolution.Width", Value)) {
                Info.m_wWidth = Value.ToInt();
            }
            if (Application.GetValue("Resolution.Height", Value)) {
                Info.m_wHeight = Value.ToInt();
            }
            if (Application.GetValue("Class", Value)) {
                app = ConstructClass<AApplication>(Value);
            }
        }
        TXMLElement Renderer;
        if (Application.GetChildElement("Renderer", Renderer)) {
            TRendererInfo RInfo;
            TXMLElement Driver;
            if (Renderer.GetChildElement("Driver", Driver)) {
                if (Driver.HasAttribute("Platform", "DirectX")) {
                    GDriver = new CDirectXDriver();
                    GSoundDriver = new CWaveIODriver();
				}
            }
        }
        TXMLElement World;
        if (Application.GetChildElement("World", World)) {
            TString Value;
            if (World.GetValue("Class", Value)) {
                app->m_pWorld = ConstructClass<UWorld>(Value);
				app->m_pWorld->m_pRenderer = app->m_pRenderer;
            } else {
                return;
			}
        }
		        
        if (app) {
			if (!app->CreateApp(Info)) {
				app->DestroyApp();
			}
            app->Initialize();
            app->Do();
            app->DestroyApp();
            delete app;
        }
    }
}
