#include "StdAfx.h"
#include "CSkeletalMeshPrimitive.h"

CSkeletalMeshPrimitive::CSkeletalMeshPrimitive(RBoneInfo* BoneInfo)
{
	if(BoneInfo)
	{
		for(unsigned int i=0;i<BoneInfo->Bones.Size();++i)
		{
			RBone* RBoneSrc = BoneInfo->Bones(i);
			TSkeletalBone* TBoneRef = new TSkeletalBone();
			CopyRBoneToTSkeletalBone(RBoneSrc, TBoneRef);
			BoneRef.AddItem(TBoneRef);
		}
	}
}

CSkeletalMeshPrimitive::~CSkeletalMeshPrimitive(void)
{
	for(unsigned int i=0;i<BoneRef.Size();++i)
	{
		delete BoneRef(i);
	}
	BoneRef.Clear();
}

void CSkeletalMeshPrimitive::CopyRBoneToTSkeletalBone(RBone *Bone, TSkeletalBone *TBone)
{
	strcpy_s(TBone->BoneName.Str, 1024, Bone->BoneName.Str);
	for(unsigned int i=0;i<BoneRef.Size();++i)
	{
		if(!strcmp(BoneRef(i)->BoneName.Str, Bone->Parent->BoneName.Str))
		{
			TBone->Parent = BoneRef(i);
		}
	}
	TBone->Rotation = Bone->Rotation;
	TBone->Scale = Bone->Scale;
	TBone->Translation = Bone->Translation;
}