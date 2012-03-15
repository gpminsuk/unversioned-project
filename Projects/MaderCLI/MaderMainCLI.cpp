#include "stdafx.h"

#include <windows.h>
#include "DMaderApp.h"
#include "MaderMainCLI.h"

INT WINAPI WinMain( HINSTANCE hInInstance, HINSTANCE hPrevInstance, char*, INT nCmdShow )
{
	DMaderApp Application;
	MaderMainCLI^ m_MaderMainCLI = gcnew MaderMainCLI(&Application);
}