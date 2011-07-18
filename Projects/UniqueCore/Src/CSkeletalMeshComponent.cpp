#include "StdAfx.h"
#include "CSkeletalMeshComponent.h"
#include "CSkeletalMeshPrimitive.h" 
#include "BDriver.h"

IMPLEMENT_CLASS(CSkeletalMeshComponent);

CSkeletalMeshComponent::CSkeletalMeshComponent()
{
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
	for(unsigned int i=0;i<Primitives(0)->Primitives.Size();++i)
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
	for(unsigned int i=0;i<Primitives(0)->Primitives.Size();++i)
	{
		TSkeletalMesh* Prim = (TSkeletalMesh*)Primitives(0)->Primitives(i);
		if(Prim)
		{
			Prim->IsPlaying = false;
		}
	}
}