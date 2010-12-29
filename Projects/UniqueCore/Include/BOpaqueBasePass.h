#pragma once

#include "BRenderPass.h"

class BRenderingBatch;

class BOpaqueBasePass : public BRenderPass
{
public:
	BOpaqueBasePass();
	~BOpaqueBasePass();

	void DrawPrimitive(BRenderingBatch *Batch);
};