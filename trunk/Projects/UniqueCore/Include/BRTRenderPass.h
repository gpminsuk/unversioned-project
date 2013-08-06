#pragma once

#include "BRenderPass.h"

class BRTRenderPass: public BRenderPass
{
public:
	BRTRenderPass();
	~BRTRenderPass();

	virtual void BeginPass(BViewport* InViewport);

	void DrawPrimitive();
};
