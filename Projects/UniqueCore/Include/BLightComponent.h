#pragma once

#include "BComponent.h"

class BViewport;
class RShader;

class BLightComponent: public BComponent
{
public:
	BLightComponent();
	virtual ~BLightComponent();

	virtual void RenderComponent(BViewport* Viewport);
	virtual void UpdateComponent() = 0;

	virtual void ConfigureShader(RShader* Shader) = 0;
};
