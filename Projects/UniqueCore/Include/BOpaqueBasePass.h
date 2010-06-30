#pragma once

#include "TDataTypes.h"

class BOpaqueBasePass
{
public:
	BOpaqueBasePass();
	~BOpaqueBasePass();

	void DrawPrimitive(TBatch *Batch);

	void BeginPass(BViewport* Viewport);
	void EndPass();

	TArray<RRenderTarget*>	m_RenderTargets;
};