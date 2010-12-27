#pragma once

#include "BRenderPass.h"

class BDrawLinePass : public BRenderPass
{
public:
	BDrawLinePass();
	~BDrawLinePass();

	void DrawPrimitive(class BLineBatcher* LineBatcher);

	void BeginPass(class BViewport* Viewport);
	void EndPass();

	class BViewport* m_Viewport;
};