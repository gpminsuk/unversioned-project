#include "stdafx.h"
#include "RSkeletalMesh.h"

IMPLEMENT_CLASS(RSkeletalMesh);
IMPLEMENT_CLASS(RSkeletalSubMesh);

bool RSkeletalSubMesh::Access(AAccessor& Accessor)
{
	return true;
}

bool RSkeletalMesh::Access(AAccessor& Accessor)
{
	Accessor << SkeletalSubMeshes;
	return true;
}