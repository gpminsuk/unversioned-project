#pragma once

#include "CMeshPrimitive.h"

#include "RResource.h"

class TSkeletalMesh
{
public:
	TSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence = NULL)
	{
		RootBone = new TBone(InBoneHierarchy->RootBone, InSkeletalMesh);
	}

	~TSkeletalMesh()
	{
		delete RootBone;
	}

	class TBone
	{
	public:
		TBone(RBoneHierarchy::RBone* InBone, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence = NULL)
		{
			BoneName = InBone->BoneName;
			for(unsigned int i=0;i<InSkeletalMesh->SkeletalSubMeshes.Size();++i)
			{	
				RSkeletalSubMesh* SubMesh = InSkeletalMesh->SkeletalSubMeshes(i);
				if(SubMesh->BoneName == BoneName)
				{
					SubMeshes.AddItem(SubMesh);
				}
			}
			for(unsigned int i=0;i<InAnimationSequence->AnimationBoneSequences.Size();++i)
			{	
				RAnimationBoneSequence* AnimSeq = InAnimationSequence->AnimationBoneSequences(i);
				if(AnimSeq->BoneName == BoneName)
				{
					AnimationBoneSequence = AnimSeq;
				}
			}
			for(unsigned int i=0;i<InBone->ChildBones.Size();++i)
			{
				TBone* Bone = new TBone(InBone->ChildBones(i), InSkeletalMesh, InAnimationSequence);
				ChildBones.AddItem(Bone);
			}
		}
		~TBone()
		{
			for(unsigned int i=0;i<ChildBones.Size();++i)
			{
				delete ChildBones(i);
			}
		}
		TString BoneName;
		TArray<RSkeletalSubMesh*> SubMeshes;
		RAnimationBoneSequence* AnimationBoneSequence;
		TArray<TBone*> ChildBones;

		TQuaternion Rotation;
		TVector3 Translation;
		float Scale;
	};

	TBone* RootBone;
};

class CSkeletalMeshPrimitive : public CMeshPrimitive
{
public:
	CSkeletalMeshPrimitive(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence);
	~CSkeletalMeshPrimitive(void);

	TSkeletalMesh *SkeletalMeshTemplate;
};
