// AA.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "crtdbg.h"
#include "UXMLParser.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//_CrtSetBreakAlloc(2679);
	UXMLApplicationParser *ApplicationConfig = new UXMLApplicationParser();

	ApplicationConfig->ReadXMLFile("..\\Config\\Application.config");
	ApplicationConfig->Parse();

	delete ApplicationConfig;
	_CrtDumpMemoryLeaks();
	return 0;
}