// AA.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "crtdbg.h"
#include "CWindowApp.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//_CrtSetBreakAlloc(6529);
	CWindowApp app;
	if(app.CreateWindowApp())
		app.Do();
	app.DestroyWindowApp();
	_CrtDumpMemoryLeaks();
	return 0;
}