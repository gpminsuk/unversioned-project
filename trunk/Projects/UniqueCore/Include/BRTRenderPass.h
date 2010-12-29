#pragma once

#include "BRenderPass.h"

class BRTRenderPass : public BRenderPass
{
public:
	BRTRenderPass();
	~BRTRenderPass();

	void DrawPrimitive();
};