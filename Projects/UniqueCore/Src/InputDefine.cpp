#include "stdafx.h"
#include "InputDefine.h"
#include <windows.h>

TMouseInput_Param::TMouseInput_Param()
{
	bRAltDown = ::GetKeyState(VK_RMENU);
	bLAltDown = ::GetKeyState(VK_LMENU);
	bRCtrlDown = ::GetKeyState(VK_RCONTROL);
	bLCtrlDown = ::GetKeyState(VK_LCONTROL);
	bRShiftDown = ::GetKeyState(VK_RSHIFT);
	bLShiftDown = ::GetKeyState(VK_LSHIFT);
}