#pragma once
#include "BCollisionBody.h"

class CCylinderCollisionBody : public BCollisionBody
{
public:
	CCylinderCollisionBody(class BThing* InOwner);
	virtual ~CCylinderCollisionBody();	

	virtual TVector3 LineCheck(TVector3 Start, TVector3 End, TVector3 Extent);
};
