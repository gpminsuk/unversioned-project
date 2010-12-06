#include "StdAfx.h"

#include "BThing.h"

#include "CCylinderCollisionBody.h"

#include "CCylinderPrimitive.h"

CCylinderCollisionBody::CCylinderCollisionBody(BThing* InOwner)
:	BCollisionBody(InOwner),
	Radius(1.0f)
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
	float DistStart = (Start - Owner->m_Location).Size();
	float DistEnd = (End - Owner->m_Location).Size();
	if(DistStart < Radius || DistEnd < Radius)
	{
		return Owner->m_Location;
	}
	return TVector3(0.0f,0.0f,0.0f);
}