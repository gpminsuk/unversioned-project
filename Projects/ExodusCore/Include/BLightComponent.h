#pragma once

#include "BComponent.h"

class BRenderer;
class RShader;

class BLightComponent: public BComponent
{
public:
	BLightComponent();
	virtual ~BLightComponent();

	virtual void RenderComponent(BRenderer* Renderer);
	virtual void UpdateComponent() = 0;

	virtual void ConfigureShader(RShader* Shader) = 0;
};
