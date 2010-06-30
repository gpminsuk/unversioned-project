#include "stdafx.h"
#include "SampleBrowserApp.h"
#include "UXMLParser.h"
#include <windows.h>

INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	UXMLApplicationParser Parser;
	Parser.ReadXMLFile("..\\..\\Config\\Application.config");
	Parser.Parse();
}