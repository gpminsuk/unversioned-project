#pragma once

#include "TDataTypes.h"

class BRenderingBatch;

class BParticleRenderPass
{
public:
	BParticleRenderPass();
	~BParticleRenderPass();

	void DrawPrimitive(BRenderingBatch *Batch);

	void BeginPass(class BViewport* Viewport);
	void EndPass();

	class BViewport* m_Viewport;
};