#include "StdAfx.h"
#include "CBoxComponent.h"
#include "CBoxPrimitive.h"

CBoxComponent::CBoxComponent(EGeometrySideType _BoxSideType) {
    CBoxPrimitive* BoxPrimitive = new CBoxPrimitive(RenderType_Opaque,
            _BoxSideType);
    Primitives.AddItem(BoxPrimitive);
}

CBoxComponent::~CBoxComponent() {
}

void CBoxComponent::UpdateComponent() {
}