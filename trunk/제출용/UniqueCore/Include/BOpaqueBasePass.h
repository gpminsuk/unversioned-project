#pragma once

#include "TDataTypes.h"

class BRenderingBatch;

class BOpaqueBasePass
{
public:
	BOpaqueBasePass();
	~BOpaqueBasePass();

	void DrawPrimitive(BRenderingBatch *Batch);

	void BeginPass(BViewport* Viewport);
	void EndPass();

	TArray<RRenderTarget*>	m_RenderTargets;
};