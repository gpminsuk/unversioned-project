#include "stdafx.h"
#include "SampleBrowserApp.h"
#include "UXMLParser.h"
#include "CCameraViewport.h"
#include <windows.h>

INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	CXMLApplicationParser Parser;
	Parser.ReadXMLFile("..\\..\\Config\\Application.config");
	Parser.Parse();
}