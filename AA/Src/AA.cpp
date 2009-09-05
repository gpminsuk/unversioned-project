// AA.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "crtdbg.h"
#include "CWindowApp.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//_CrtSetBreakAlloc(1010);
	BApplication *app = new CWindowApp();
	if(app->CreateApp())
		app->Do();
	app->DestroyApp();
	delete app;
	_CrtDumpMemoryLeaks();
	return 0;
}