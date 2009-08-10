#pragma once

#include "CMeshPrimitive.h"

#include "RResource.h"

class TSkeletalBone
{
public:
	TSkeletalBone() : Parent(0) {}
	TString BoneName;
	TSkeletalBone *Parent;

	TQuaternion Rotation;
	TVector3 Translation;
	float Scale;
};

class CSkeletalMeshPrimitive : public CMeshPrimitive
{
public:
	CSkeletalMeshPrimitive(RBoneInfo* BoneInfo = NULL);
	~CSkeletalMeshPrimitive(void);

	class RBoneInfo* BoneInfo;
	TArray<TSkeletalBone*> BoneRef;
	TArray<class RAnimationSequence*> Animations;

private:
	void CopyRBoneToTSkeletalBone(RBone* Bone, TSkeletalBone* TBone);
};
