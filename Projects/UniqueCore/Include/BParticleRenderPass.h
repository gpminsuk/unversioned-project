#pragma once

#include "BRenderPass.h"

class BRenderingBatch;

class BParticleRenderPass : public BRenderPass
{
public:
	BParticleRenderPass();
	~BParticleRenderPass();

	void DrawPrimitive(BRenderingBatch *Batch);

	class BViewport* m_Viewport;
};