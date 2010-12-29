#pragma once

#include "BRenderPass.h"

class BRenderingBatch;
class BRenderingBatchChunk;
class BViewport;
class RRenderTarget;
class BLight;

class BDirectionalLightPass : public BRenderPass
{
public:
	BDirectionalLightPass();
	~BDirectionalLightPass();

	virtual void BeginPass(BViewport* InViewport);

	void DrawPrimitive(BRenderingBatch *Batch);
};