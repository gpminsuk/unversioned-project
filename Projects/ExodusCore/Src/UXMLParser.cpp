#include "stdafx.h"
#include "UXMLParser.h"

#include "BRenderer.h"

#include "CWindowApp.h"
#include "CCameraViewport.h"
#include "CDirectXDriver.h"
#include "CWaveIODriver.h"

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
        if (Application.HasAttribute("Platform", "Windows_x86")) {
            TWindowInfo Info;
            TString Value;
            if (Application.GetValue("Resolution.Width", Value)) {
                Info.m_wWidth = Value.ToInt();
            }
            if (Application.GetValue("Resolution.Height", Value)) {
                Info.m_wHeight = Value.ToInt();
            }
            if (Application.GetValue("Class", Value)) {
                app = ConstructClass<AApplication>(Value);
            } else {
                return;
            }
            if (!app->CreateApp(Info)) {
                app->DestroyApp();
                app = 0;
            }
        }
        TXMLElement Renderer;
        if (Application.GetChildElement("Renderer", Renderer)) {
            TRendererInfo Info;
            TXMLElement Driver;
            if (Renderer.GetChildElement("Driver", Driver)) {
                if (Driver.HasAttribute("Platform", "DirectX")) {
                    CWindowApp* WindowsApp = static_cast<CWindowApp*>(app);
                    if (WindowsApp) {
                        TWindowInfo& WindowInfo = (TWindowInfo&) WindowsApp
                                                  ->m_WindowInfo;
                        TDXWindowInfo DXWindowInfo;
                        DXWindowInfo.m_hWnd = WindowInfo.m_hWnd;
                        DXWindowInfo.m_wHeight = WindowInfo.m_wHeight;
                        DXWindowInfo.m_wWidth = WindowInfo.m_wWidth;
                        GDriver = new CDirectXDriver(DXWindowInfo);
                        GDriver->CreateDriver();

                        GSoundDriver = new CWaveIODriver();
                    }
                }
            }
            TString Value;
            app->m_pRenderer = new BRenderer(app);
        }
        TXMLElement World;
        if (Application.GetChildElement("World", World)) {
            TString Value;
            if (World.GetValue("Class", Value)) {
                app->m_pWorld = ConstructClass<UWorld>(Value);
            } else {
                return;
            }
            TXMLElement Viewport;
            if (World.GetChildElement("Viewport", Viewport)) {
                if (Viewport.GetValue("Class", Value)) {
                    app->m_pViewport = ConstructClass<BViewport>(Value);
                    app->m_pRenderer->AddViewport(app->m_pViewport);
                    app->m_pWorld->AddViewport(app->m_pViewport);
                } else {
                    return;
                }
            }
        }

        if (app) {
            app->Initialize();
            app->Do();
            app->DestroyApp();
            delete app;
        }
    }
}
