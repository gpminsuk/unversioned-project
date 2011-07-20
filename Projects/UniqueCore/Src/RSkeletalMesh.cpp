#include "stdafx.h"
#include "RSkeletalMesh.h"

IMPLEMENT_CLASS(RSkeletalMesh);
IMPLEMENT_CLASS(RSkeletalSubMesh);

bool RSkeletalSubMesh::Access(AAccessor& Accessor)
{
	__super::Access(Accessor);

	Accessor << BoneName;
	Accessor << pIB;
	Accessor << pVB;
	return true;
}

bool RSkeletalMesh::Access(AAccessor& Accessor)
{
	__super::Access(Accessor);

	Accessor << SkeletalSubMeshes;
	return true;
}