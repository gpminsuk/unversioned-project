#pragma once

#include "TDataTypes.h"

class BRenderingBatch;

class BDrawUIPass
{
public:
	BDrawUIPass();
	~BDrawUIPass();

	void DrawPrimitive(BRenderingBatch *Batch);

	void BeginPass(class BViewport* Viewport);
	void EndPass();

	class BViewport* m_Viewport;
};