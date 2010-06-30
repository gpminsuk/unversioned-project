#pragma once

#include "TDataTypes.h"

class BDrawLinePass
{
public:
	BDrawLinePass();
	~BDrawLinePass();

	void DrawPrimitive(class BLineBatcher* LineBatcher);

	void BeginPass(class BViewport* Viewport);
	void EndPass();

	class BViewport* m_Viewport;
};