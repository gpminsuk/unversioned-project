#include "StdAfx.h"

#include "BThing.h"

#include "CHeightFieldCollisionBody.h"

#include "CCylinderPrimitive.h"

CHeightFieldCollisionBody::CHeightFieldCollisionBody(BThing* InOwner)
:	BCollisionBody(InOwner)
{
	CCylinderPrimitive *CylinderPrimitive = new CCylinderPrimitive();
	Primitives.AddItem(CylinderPrimitive);
}

CHeightFieldCollisionBody::~CHeightFieldCollisionBody()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		delete Primitives(i);
	}
	Primitives.Clear();
}

TVector3 CHeightFieldCollisionBody::LineCheck(TVector3& Start, TVector3& End, TVector3& Extent)
{
	return TVector3(End.x, (End.y<0)?0:End.y, End.z);
}