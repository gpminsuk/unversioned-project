#include "StdAfx.h"
#include "BLightComponent.h"
#include "BPrimitive.h"
#include "BRenderer.h"

BLightComponent::BLightComponent() {
}

BLightComponent::~BLightComponent() {
}

void BLightComponent::RenderComponent(BRenderer* Renderer) {
    Renderer->RenderLight(this);
}
