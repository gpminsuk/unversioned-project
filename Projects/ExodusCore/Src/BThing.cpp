#include "StdAfx.h"
#include "BThing.h"

#include "BPrimitive.h"
#include "BCollisionBody.h"

BThing::BThing(void) {
}

BThing::~BThing(void) {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        delete Primitives(i);
    }
    Primitives.Clear();

    for (unsigned int i = 0; i < CollisionBodies.Size(); ++i) {
        delete CollisionBodies(i);
    }
    CollisionBodies.Clear();
}

TVector3 BThing::LineCheck(TVector3& Start, TVector3& End, TVector3& Extent) {
    TArray<TVector3> HitPositions;
    TVector3 HitPosition;
    for (unsigned int i = 0; i < CollisionBodies.Size(); ++i) {
        HitPosition = CollisionBodies(i)->LineCheck(Start, End, Extent);
        HitPositions.AddItem(HitPosition);
    }
    if (HitPositions.Size() == 0) {
        return TVector3(0, 0, 0);
    }
    return HitPositions(0);
}

bool BThing::Access(AAccessor& Accessor) {
	__super::Access(Accessor);

	Accessor << m_Scale;
	Accessor << m_Location;
	Accessor << Primitives;
	return true;
}

bool BThing::Tick(unsigned long dTime) {
    return true;
}