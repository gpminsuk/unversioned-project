#pragma once

#include "BComponent.h"

class BViewport;

class BLightComponent : public BComponent
{
public:
	BLightComponent();
	virtual ~BLightComponent();

	virtual void RenderComponent(BViewport* Viewport);
	virtual void UpdateComponent() = 0;
};