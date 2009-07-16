#include "stdafx.h"

#include <windows.h>
#include "CMaderApp.h"
#include "MaderMainCLI.h"

INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	CMaderApp Application;
	MaderMainCLI^ m_MaderMainCLI = gcnew MaderMainCLI(&Application);
}