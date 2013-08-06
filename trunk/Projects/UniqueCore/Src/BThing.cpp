#include "StdAfx.h"
#include "BThing.h"

#include "BPrimitive.h"
#include "BCollisionBody.h"
#include "BComponent.h"

BThing::BThing(void) {
}

BThing::~BThing(void) {
    for (unsigned int i = 0; i < Components.Size(); ++i) {
        delete Components(i);
    }
    Components.Clear();

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

bool BThing::Tick(unsigned long dTime) {
    for (unsigned int i = 0; i < Components.Size(); ++i) {
        BComponent* Comp = Components(i);
        Comp->UpdateComponent();
    }
    return true;
}

TArray<BPrimitive*> BThing::GetPrimitives() {
    TArray<BPrimitive*> Ret;
    for (unsigned int i = 0; i < Components.Size(); ++i) {
        BComponent* CP = Components(i);
        Ret.AddItems(CP->Primitives);
    }
    for (unsigned int i = 0; i < CollisionBodies.Size(); ++i) {
        BCollisionBody* CB = CollisionBodies(i);
        Ret.AddItems(CB->Primitives);
    }
    return Ret;
}
