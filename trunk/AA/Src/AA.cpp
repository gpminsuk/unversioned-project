// AA.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "CWindowApp.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CWindowApp app;
	if(app.CreateWindowApp())
		app.Do();
	return 0;
}

