#include "stdafx.h"
#include "BPrimitive.h"
#include "CMissile.h"
#include "CBoxComponent.h"

CMissile::CMissile()
{
	CBoxComponent* BoxComponent = new CBoxComponent();
	Components.AddItem(BoxComponent);
}

CMissile::~CMissile()
{

}

void CMissile::Tick(unsigned long dTime)
{

}

void CMissile::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{

}

void CMissile::PhysicsTick(unsigned long dTime)
{

}

void CMissile::UpdateTransform()
{
	for(unsigned int i=0;i<Components(0)->Primitives.Size();++i)
	{
		Components(0)->Primitives(i)->Translation = m_Location;
		Components(0)->Primitives(i)->TM._41 = m_Location.x;
		Components(0)->Primitives(i)->TM._42 = m_Location.y;
		Components(0)->Primitives(i)->TM._43 = m_Location.z;
	}
}