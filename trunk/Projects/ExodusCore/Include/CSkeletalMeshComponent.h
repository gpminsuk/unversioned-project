#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CSkeletalMeshPrimitive;
class RSkeletalMesh;
class RBoneHierarchy;
class RAnimationSequence;
class RAnimationController;

class CAnimationController: public AObject {
	DECLARE_CLASS(CAnimationController,)

	RAnimationController* AnimationController;
public:
	void SetAnimationController(RAnimationController* InAnimationController);
	void PlayAnimation(RAnimationSequence* AnimationSequence);
};

class CSkeletalMeshComponent: public BComponent
{
DECLARE_CLASS(CSkeletalMeshComponent,)
	public:
	CSkeletalMeshComponent();
	virtual ~CSkeletalMeshComponent();

	CAnimationController* AnimationController;
	CSkeletalMeshPrimitive* SkeletalMeshPrimitive;
	void SetSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* SkeletalMesh, RAnimationSequence* AnimationSeq);

	virtual void UpdateComponent();

	void PlayAnimation(int StartTime, bool Loop);
	void StopAnimation();
};
