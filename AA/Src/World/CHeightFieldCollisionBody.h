#pragma once
#include "BCollisionBody.h"

class CHeightFieldCollisionBody : public BCollisionBody
{
public:
	CHeightFieldCollisionBody(class BThing* InOwner);
	virtual ~CHeightFieldCollisionBody();	

	virtual TVector3 LineCheck(TVector3 Start, TVector3 End, TVector3 Extent);
};
