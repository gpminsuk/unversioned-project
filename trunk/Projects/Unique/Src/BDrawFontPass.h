#pragma once

#include "TDataTypes.h"

class BDrawFontPass
{
public:
	BDrawFontPass();
	~BDrawFontPass();

	void DrawPrimitive(float fps);

	void BeginPass(class BViewport* Viewport);
	void EndPass();

	class BViewport* m_Viewport;
};