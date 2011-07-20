#include "stdafx.h"
#include "RBoneHierarchy.h"

IMPLEMENT_CLASS(RBoneHierarchy);
IMPLEMENT_CLASS(RBone);

bool RBone::Access(AAccessor& Accessor)
{
	__super::Access(Accessor);

	Accessor << BoneName;
	Accessor << ChildBones;
	if(Accessor.IsLoading())
	{
		for(unsigned int i=0;i<ChildBones.Size();++i)
		{
			ChildBones[i]->Parent = this;
		}
	}
	Accessor << TM;
	Accessor << InvTM;

	return true;
}

bool RBoneHierarchy::Access(AAccessor& Accessor)
{
	__super::Access(Accessor);

	Accessor << RootBone;
	return true;
}