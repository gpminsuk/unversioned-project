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

	void DrawPrimitive(BRenderingBatch *Batch);

	void BeginRenderLight(BLightComponent *InLight);
};