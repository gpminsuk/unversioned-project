#pragma once

#include "CWindowApp.h"
#include <vcclr.h>

ref class MaderMainCLI;

class DMaderApp : public CWindowApp
{
	DECLARE_CLASS(DMaderApp,)
public:
	DMaderApp();
	~DMaderApp();

	gcroot<MaderMainCLI^> m_MaderMainCLI;

	virtual bool CreateApp(TApplicationInfo& Info);
	virtual void Do();
};
