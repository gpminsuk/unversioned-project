#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CSkeletalMeshPrimitive;
class RSkeletalMesh;
class RBoneHierarchy;
class RAnimationSequence;

class CSkeletalMeshComponent: public BComponent
{
DECLARE_CLASS(CSkeletalMeshComponent,)
	public:
	CSkeletalMeshComponent();
	virtual ~CSkeletalMeshComponent();

	CSkeletalMeshPrimitive* SkeletalMeshPrimitive;
	void SetSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* SkeletalMesh, RAnimationSequence* AnimationSeq);

	virtual void UpdateComponent();

	void PlayAnimation(int StartTime, bool Loop);
	void StopAnimation();
};
