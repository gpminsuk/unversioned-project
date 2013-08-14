#include "stdafx.h"
#include "RMesh.h"

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
