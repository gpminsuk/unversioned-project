#pragma once

#include "AObject.h"
#include "TDataTypes.h"
#include "RResource.h"
#include "RAsset.h"
#include "RAnimationSequence.h"

class RBone: public AObject
{
DECLARE_CLASS(RBone,)
	public:
	TString BoneName;

	int BoneIndex;
	int SkinBoneIndex;
	int ParentIndex;
	RBone *Parent;
	
	TMatrix TM;
	TMatrix InvTM;

	virtual bool Access(AAccessor& Accessor);
};

class RBoneHierarchy: public RAsset
{
DECLARE_CLASS(RBoneHierarchy,)
	public:
	TMatrix GetBoneMatrix(RBone* Bone)
			{
		TMatrix Ret;
		Ret.SetIdentity();
		while (Bone)
		{
			Ret = Ret * Bone->TM;
			Bone = Bone->Parent;
		}
		return Ret;
	}

	~RBoneHierarchy()
	{
		for (unsigned int i = 0; i < Bones.Size(); ++i)
				{
			delete Bones(i);
		}
	}

	TArray<RBone*> Bones;

	RBone* FindBone(TString& name);
	virtual bool Access(AAccessor& Accessor);

	virtual TArray<TString> GetCreatablePrimitiveClassNames()
	{
		TArray<TString> Ret;
		Ret.AddItem(TString("CSkeletalMeshPrimitive"));
		return Ret;
	}
};
