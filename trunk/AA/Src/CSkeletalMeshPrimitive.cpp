#include "StdAfx.h"
#include "CSkeletalMeshPrimitive.h"

CSkeletalMeshPrimitive::CSkeletalMeshPrimitive(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence)
{
	SkeletalMeshTemplate = new TSkeletalMesh(InBoneHierarchy, InSkeletalMesh, InAnimationSequence);
}

CSkeletalMeshPrimitive::~CSkeletalMeshPrimitive(void)
{
	delete SkeletalMeshTemplate;
}