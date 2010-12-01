#include "stdafx.h"
#include "BPrimitive.h"
#include "CMissile.h"
#include "CBoxComponent.h"
#include "CCylinderCollisionBody.h"
#include "UWorld.h"

CMissile::CMissile()
{
	CBoxComponent* BoxComponent = new CBoxComponent();
	Components.AddItem(BoxComponent);

	CCylinderCollisionBody* CylinderCollisionBody = new CCylinderCollisionBody(this);
	CollisionBodies.AddItem(CylinderCollisionBody);

	m_Location = TVector3(10.0f, 10.0f, 10.0f);
	UpdateTransform();
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
	TVector3 Loc = m_Location;
	float t = dTime/100.f;
	Loc.y -= (float)(t*t*9.8/2.0f);
	TVector3 Hit = GWorld->LineCheck(this, m_Location, Loc).HitPosition;
	if(Hit != TVector3(0,0,0))
		m_Location = Hit;
	else
		m_Location = Loc;
	UpdateTransform();
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
	for(unsigned int i=0;i<CollisionBodies.Size();++i)
	{
		for(unsigned int j=0;j<Components(i)->Primitives.Size();++j)
		{
			Components(i)->Primitives(j)->Translation = m_Location;
			Components(i)->Primitives(j)->TM = TMatrix(TVector3(m_Location.x, m_Location.y, m_Location.z),
				TQuaternion(TVector3(1.0f,0.0f,0.0f),0.f), 1.0f);
		}
	}
	CollisionBodyBounds.Box.Extent = TVector3(5.0f,5.0f,5.0f);
	CollisionBodyBounds.Position = m_Location;
	CollisionBodyBounds.Sphere.Radius = 5.0f;	
}