#pragma once

#include "BRenderPass.h"

class BRTRenderPass : public BRenderPass
{
public:
	BRTRenderPass();
	~BRTRenderPass();

	void DrawPrimitive();

	void BeginPass(BViewport* Viewport);
	void EndPass();
};