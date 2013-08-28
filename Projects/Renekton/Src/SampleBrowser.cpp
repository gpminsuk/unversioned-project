#include "stdafx.h"
#include "UXMLParser.h"
#include <windows.h>

INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	CXMLApplicationParser Parser;
	Parser.ReadXMLFile("..\\..\\Config\\Renekton.config");
	Parser.Parse();
} 