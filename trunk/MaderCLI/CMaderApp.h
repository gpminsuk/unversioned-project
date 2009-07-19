#pragma once

#include "..\AA\Src\TWindowInfo.h"
#include "..\AA\Src\BApplication.h"

class CMaderApp : public BApplication
{
public:
	int m_wWidth;
	int m_wHeight;

	TWindowInfo WindowInfo;

	bool CreateMaderApp();
	void Do();
};
