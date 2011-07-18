#pragma once

#include "AObject.h"
#include "TDataTypes.h"

class RBoneHierarchy : public AObject
{
	DECLARE_CLASS(RBoneHierarchy,)
public:
	class RBone
	{
	public:
		RBone() : Parent(0) {}
		~RBone()
		{
			for(unsigned int i=0;i<ChildBones.Size();++i)
			{
				delete ChildBones(i);
			}
		}

		void AddBone_Recursive(RBone* Bone, TString ParentName)
		{
			if(BoneName == ParentName)
			{
				ChildBones.AddItem(Bone);
				Bone->Parent = this;
			}
			else
			{
				for(unsigned int i=0;i<ChildBones.Size();++i)
				{
					ChildBones(i)->AddBone_Recursive(Bone, ParentName);
				}
			}			
		}

		TString BoneName;
		RBone *Parent;
		TArray<RBone*> ChildBones;

		TMatrix TM;
		TMatrix InvTM;
	};

	TMatrix GetBoneMatrix(RBone* Bone)
	{
		TMatrix Ret;
		Ret.SetIdentity();
		while(Bone)
		{
			Ret = Ret * Bone->TM;
			Bone = Bone->Parent;
		}
		return Ret;
	}

	void AddBone(RBone* Bone, TString ParentName)
	{
		if(ParentName == "")
		{
			RootBone.AddItem(Bone);
		}
		else
		{
			RootBone(0)->AddBone_Recursive(Bone, ParentName);
		}
	}

	RBoneHierarchy()
	{
	}

	~RBoneHierarchy()
	{
		for(unsigned int i=0;i<RootBone.Size();++i)
		{
			delete RootBone(i);
		}
	}

	TArray<RBone*> RootBone;
};