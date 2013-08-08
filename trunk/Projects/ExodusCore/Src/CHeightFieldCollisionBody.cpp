#include "StdAfx.h"

#include "BThing.h"

#include "CHeightFieldCollisionBody.h"

CHeightFieldCollisionBody::CHeightFieldCollisionBody(BThing* InOwner)
    :
    BCollisionBody(InOwner) {
}

CHeightFieldCollisionBody::~CHeightFieldCollisionBody() {
}

TVector3 CHeightFieldCollisionBody::LineCheck(TVector3& Start, TVector3& End, TVector3& Extent) {
    return TVector3(End.x, (End.y < 0) ? 0 : End.y, End.z);
}
