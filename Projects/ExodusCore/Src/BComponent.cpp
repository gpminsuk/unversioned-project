#include "StdAfx.h"
#include "BComponent.h"
#include "BPrimitive.h"
#include "BRenderer.h"

BComponent::BComponent() {
}

BComponent::~BComponent() {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        delete Primitives(i);
    }
    Primitives.Clear();
}

void BComponent::RenderComponent(BRenderingBatchManager* BatchManager, BRenderer* Renderer) {
    for (unsigned int j = 0; j < Primitives.Size(); ++j) {
        Renderer->Render(BatchManager, Primitives(j));
    }
}
