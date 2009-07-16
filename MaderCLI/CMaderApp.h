#pragma once

#include "..\Src\BApplication.h"

class CMaderApp : public BApplication
{
public:
	int m_wWidth;
	int m_wHeight;

	bool CreateMaderApp();
};
