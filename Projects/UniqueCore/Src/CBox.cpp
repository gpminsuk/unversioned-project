#include "StdAfx.h"
#include "CBox.h"

#include "BPrimitive.h"

#include "CBoxComponent.h"

CBox::CBox(EGeometrySideType _BoxSideType) {
    CBoxComponent* BoxComponent = new CBoxComponent(_BoxSideType);
    Components.AddItem(BoxComponent);
}

CBox::~CBox() {

}

bool CBox::Tick(unsigned long dTime) {
    return true;
}

void CBox::PhysicsTick(unsigned long dTime) {
}

void CBox::UpdateTransform() {
    for (unsigned int i = 0; i < Components(0)->Primitives.Size(); ++i) {
        Components(0)->Primitives(i)->Translation = m_Location;
        Components(0)->Primitives(i)->TM._41 = m_Location.x;
        Components(0)->Primitives(i)->TM._42 = m_Location.y;
        Components(0)->Primitives(i)->TM._43 = m_Location.z;

        Components(0)->Primitives(i)->TM._11 *= m_Scale.x;
        Components(0)->Primitives(i)->TM._22 *= m_Scale.y;
        Components(0)->Primitives(i)->TM._33 *= m_Scale.z;
    }
}
