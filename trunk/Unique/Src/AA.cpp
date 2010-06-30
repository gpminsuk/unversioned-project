// AA.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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