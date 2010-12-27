#pragma once

#include "BRenderPass.h"

class BRenderingBatch;
class BViewport;
class RRenderTarget;
class BLight;

class BDirectionalLightPass : public BRenderPass
{
public:
	BDirectionalLightPass();
	~BDirectionalLightPass();

	void DrawPrimitive(BRenderingBatch *Batch);

	void BeginPass(BViewport* Viewport, BLight* Light);
	void EndPass();
};