#include "stdafx.h"

#include "BPrimitive.h"

#include "CTank.h"
#include "CBoxComponent.h"

CTank::CTank() :
	IsInTurn(false)
{
	CBoxComponent* BoxComponent = new CBoxComponent();
	Components.AddItem(BoxComponent);
}

CTank::~CTank()
{

}

void CTank::Fire()
{

}

void CTank::Tick(unsigned long dTime)
{
	if(IsInTurn)
	{
		if(GKeyMap[VK_LEFT])
		{
			m_Location.x += 0.01f;
			UpdateTransform();
		}
		if(GKeyMap[VK_RIGHT])
		{
			m_Location.x -= 0.01f;
			UpdateTransform();
		}
	}	
}

void CTank::PhysicsTick(unsigned long dTime)
{

}

void CTank::UpdateTransform()
{
	for(unsigned int i=0;i<Components(0)->Primitives.Size();++i)
	{
		Components(0)->Primitives(i)->Translation = m_Location;
		Components(0)->Primitives(i)->TM._41 = m_Location.x;
		Components(0)->Primitives(i)->TM._42 = m_Location.y;
		Components(0)->Primitives(i)->TM._43 = m_Location.z;
	}
}