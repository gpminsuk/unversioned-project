#pragma once

#include "BShaderPass.h"

class BOpaqueBasePass : public BShaderPass
{
public:
	BOpaqueBasePass();
	virtual ~BOpaqueBasePass();

	virtual void DrawPrimitive(BViewport* Viewport, TPrimitiveTemplateBase* Prim);
};