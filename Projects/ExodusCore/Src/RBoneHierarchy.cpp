#include "stdafx.h"
#include "RBoneHierarchy.h"

bool RBone::Access(AAccessor& Accessor) {
    __super::Access(Accessor);

    Accessor << BoneName;
    Accessor << TM;
    Accessor << InvTM;
	Accessor << BoneIndex;
	Accessor << SkinBoneIndex;
	Accessor << ParentIndex;

    return true;
}

bool RBoneHierarchy::Access(AAccessor& Accessor) {
    __super::Access(Accessor);

    Accessor << Bones;
    return true;
}

RBone* RBoneHierarchy::FindBone(TString& name) {
    for (unsigned int i = 0; i < Bones.Size(); ++i) {
        if (Bones(i)->BoneName == name) {
            return Bones(i);
        }
    }
    return 0;
}
