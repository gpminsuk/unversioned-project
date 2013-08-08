#pragma once

#include "BRenderPass.h"

class BRenderingBatch;

class BDrawUIPass: public BRenderPass
{
public:
	BDrawUIPass();
	~BDrawUIPass();

	void DrawPrimitive(BRenderingBatch *Batch);
};
