#pragma once

#include "TDataTypes.h"

class BCollisionBody
{
public:
	BCollisionBody(class BThing* InOwner);
	virtual ~BCollisionBody();

	TArray<class BPrimitive*> Primitives;

	class BThing* Owner;

	virtual TVector3 LineCheck(TVector3 Start, TVector3 End, TVector3 Extent) = 0;
};
