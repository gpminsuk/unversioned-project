#pragma once

#include "BRenderPass.h"

class BDrawLinePass: public BRenderPass
{
public:
	BDrawLinePass();
	~BDrawLinePass();

	void DrawPrimitive(class BLineBatcher* LineBatcher);
};
