#include "StdAfx.h"
#include "BCollisionBody.h"
#include "BPrimitive.h"

BCollisionBody::BCollisionBody(BThing* InOwner)
:	Owner(InOwner)
{
}

BCollisionBody::~BCollisionBody()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		delete Primitives(i);
	}
	Primitives.Clear();	
}