#include "StdAfx.h"

#include "BThing.h"

#include "CCylinderCollisionBody.h"

#include "CCylinderPrimitive.h"

CCylinderCollisionBody::CCylinderCollisionBody(BThing* InOwner)
:	BCollisionBody(InOwner),
	Radius(1.0f),
	Height(2.0f)
{
	CCylinderPrimitive *CylinderPrimitive = new CCylinderPrimitive();
	Primitives.AddItem(CylinderPrimitive);
}

CCylinderCollisionBody::~CCylinderCollisionBody()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		delete Primitives(i);
	}
	Primitives.Clear();
}

TVector3 CCylinderCollisionBody::LineCheck(TVector3& Start, TVector3& End, TVector3& Extent)
{
	TVector2 Start2D = TVector2(Start.x, Start.z);
	TVector2 End2D = TVector2(End.x, End.z);
	TVector2 Pos2D = TVector2(Owner->m_Location.x, Owner->m_Location.z);
	float DistStart = (Start2D - Pos2D).Size();
	float DistEnd = (End2D - Pos2D).Size();
	if((DistStart < Radius || DistEnd < Radius) && ((Start.y < Pos2D.y + Height/2 && Start.y > Pos2D.y) || (End.y < Pos2D.y + Height/2 && End.y > Pos2D.y)))
	{
		return Owner->m_Location;
	}
	return TVector3(0.0f,0.0f,0.0f);
}