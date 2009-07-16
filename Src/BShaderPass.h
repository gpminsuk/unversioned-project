#pragma once

class BViewport;
class TPrimitiveTemplateBase;

class BShaderPass
{
public:
	BShaderPass();
	virtual ~BShaderPass();

	virtual void DrawPrimitive(BViewport* Viewport, TPrimitiveTemplateBase* Prim);
};