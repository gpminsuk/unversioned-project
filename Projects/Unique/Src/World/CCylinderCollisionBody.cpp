#include "StdAfx.h"

#include "BThing.h"

#include "CCylinderCollisionBody.h"

#include "CCylinderPrimitive.h"

CCylinderCollisionBody::CCylinderCollisionBody(BThing* InOwner)
:	BCollisionBody(InOwner)
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
	return TVector3(Owner->m_Location.x, 0, Owner->m_Location.z);
}