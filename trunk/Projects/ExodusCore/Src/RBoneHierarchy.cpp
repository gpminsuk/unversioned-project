#include "stdafx.h"
#include "RBoneHierarchy.h"

IMPLEMENT_CLASS(RBoneHierarchy);
IMPLEMENT_CLASS(RBone);

bool RBone::Access(AAccessor& Accessor) {
    __super::Access(Accessor);

    Accessor << BoneName;
    Accessor << TM;
    Accessor << InvTM;

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
