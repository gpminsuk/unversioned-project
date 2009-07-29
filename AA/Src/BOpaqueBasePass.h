#pragma once

#include "TDataTypes.h"

class BOpaqueBasePass
{
public:
	BOpaqueBasePass();
	~BOpaqueBasePass();

	void DrawPrimitive(TPrimitiveTemplateBase* Prim);

	void BeginPass(BViewport* Viewport);
	void EndPass();

	TArray<RRenderTarget*>	m_RenderTargets;
};