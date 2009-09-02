#pragma once

#include "AObject.h"
#include "TDataTypes.h"

class BThing : public AObject
{
public:
	BThing(void);
	virtual ~BThing(void);

	TArray<class BPrimitive*> Primitives;
	TBounds PrimitiveBounds;
	TArray<class BCollisionBody*> CollisionBodies;
	TBounds CollisionBodyBounds;

	TVector3 m_Location;

	virtual void Tick(unsigned long dTime) = 0;
	virtual void PhysicsTick(unsigned long dTime) = 0;
	virtual void UpdateTransform() = 0;
	virtual TVector3 LineCheck(TVector3 Start, TVector3 End, TVector3 Extent);
};
