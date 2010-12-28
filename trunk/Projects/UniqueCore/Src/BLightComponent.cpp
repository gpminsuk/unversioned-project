#include "StdAfx.h"
#include "BLightComponent.h"
#include "BPrimitive.h"
#include "BViewport.h"

BLightComponent::BLightComponent()
{	
}

BLightComponent::~BLightComponent()
{
}

void BLightComponent::RenderComponent(BViewport* Viewport)
{
	Viewport->RenderLight(this);
}