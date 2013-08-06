#pragma once

#include "BRenderPass.h"

class BRenderingBatch;

class BOpaqueBasePass: public BRenderPass
{
public:
	BOpaqueBasePass();
	~BOpaqueBasePass();

	virtual void BeginPass(BViewport* InViewport);

	void DrawPrimitive(BRenderingBatch *Batch);
};
