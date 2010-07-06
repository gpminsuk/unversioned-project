#include "StdAfx.h"
#include "CSkeletalMeshComponent.h"
#include "CSkeletalMeshPrimitive.h" 

CSkeletalMeshComponent::CSkeletalMeshComponent()
{
	CSkeletalMeshPrimitive* SkeletalMeshPrimitive = new CSkeletalMeshPrimitive(RBoneHierarchyTable::BoneHierarchies(0), RSkeletalMeshTable::SkeletalMeshes(0), RAnimationSequenceTable::Sequences(0));
	Primitives.AddItem(SkeletalMeshPrimitive);	
}

CSkeletalMeshComponent::~CSkeletalMeshComponent()
{
}

void CSkeletalMeshComponent::UpdateComponent()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{		
	}
}