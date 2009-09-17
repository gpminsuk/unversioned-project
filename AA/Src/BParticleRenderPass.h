#pragma once

#include "TDataTypes.h"

class BParticleRenderPass
{
public:
	BParticleRenderPass();
	~BParticleRenderPass();

	void DrawPrimitive(TBatch *Batch);

	void BeginPass(class BViewport* Viewport);
	void EndPass();

	class BViewport* m_Viewport;
};