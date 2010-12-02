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

void CSkeletalMeshComponent::PlayAnimation(int StartTime, bool Loop)
{
	for(int i=0;i<Primitives(0)->Primitives.Size();++i)
	{
		TSkeletalMesh* Prim = (TSkeletalMesh*)Primitives(0)->Primitives(i);
		if(Prim)
		{
			Prim->IsLooping = Loop;
			Prim->CurrentFrame = StartTime;
			Prim->IsPlaying = true;
		}
	}
}

void CSkeletalMeshComponent::StopAnimation()
{
	for(int i=0;i<Primitives(0)->Primitives.Size();++i)
	{
		TSkeletalMesh* Prim = (TSkeletalMesh*)Primitives(0)->Primitives(i);
		if(Prim)
		{
			Prim->IsPlaying = false;
		}
	}
}