#include "stdafx.h"
#include "SampleBrowserApp.h"
#include "MyXMLParser.h"
#include <windows.h>

INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	MyXMLParser Parser;
	Parser.ReadXMLFile("..\\..\\Config\\Application.config");
	Parser.Parse();
}