#include "StdAfx.h"
#include "BComponent.h"
#include "BPrimitive.h"
#include "BViewport.h"

BComponent::BComponent() {
}

BComponent::~BComponent() {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        delete Primitives(i);
    }
    Primitives.Clear();
}

void BComponent::RenderComponent(BViewport* Viewport) {
    for (unsigned int j = 0; j < Primitives.Size(); ++j) {
        Viewport->Render(Primitives(j));
    }
}
