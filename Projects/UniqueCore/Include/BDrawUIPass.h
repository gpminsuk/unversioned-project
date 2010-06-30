#pragma once

#include "TDataTypes.h"

class BDrawUIPass
{
public:
	BDrawUIPass();
	~BDrawUIPass();

	void DrawPrimitive();

	void BeginPass(class BViewport* Viewport);
	void EndPass();

	class BViewport* m_Viewport;
};