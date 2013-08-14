#include "stdafx.h"
#include "RMesh.h"

IMPLEMENT_CLASS(RMesh);
IMPLEMENT_CLASS(RSubMesh);

bool RSubMesh::Access(AAccessor& Accessor) {
    __super::Access(Accessor);

    Accessor << BoneName;
    Accessor << pIB;
    Accessor << pVB;
    return true;
}

bool RMesh::Access(AAccessor& Accessor) {
    __super::Access(Accessor);

    Accessor << SkeletalSubMeshes;
    return true;
}
