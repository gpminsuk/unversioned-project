#include "StdAfx.h"
#include "CBox.h"

#include "BPrimitive.h"
#include "CBoxPrimitive.h"

IMPLEMENT_CLASS(CBox);

CBox::CBox(EGeometrySideType _BoxSideType) {
    Primitives.AddItem(new CBoxPrimitive(RenderType_Opaque, _BoxSideType));
}

CBox::~CBox() {

}

bool CBox::Tick(unsigned long dTime) {
    return true;
}

void CBox::PhysicsTick(unsigned long dTime) {
}

void CBox::UpdateTransform() {
    for (unsigned int i = 0; i < Primitives(0)->Draws.Size(); ++i) {
        Primitives(i)->Translation = m_Location;
        Primitives(i)->TM._41 = m_Location.x;
        Primitives(i)->TM._42 = m_Location.y;
        Primitives(i)->TM._43 = m_Location.z;

        Primitives(i)->TM._11 *= m_Scale.x;
        Primitives(i)->TM._22 *= m_Scale.y;
        Primitives(i)->TM._33 *= m_Scale.z;
    }
}
