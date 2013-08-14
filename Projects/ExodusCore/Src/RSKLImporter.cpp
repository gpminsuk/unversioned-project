#include "stdafx.h"
#include "RSKLImporter.h"

#include "RBoneHierarchy.h"

bool RSKLImporter::Import(TString& Filename, RAnimationSequence*& AnimationSequence, RBoneHierarchy*& BoneHierarchy, RMesh*& Model) {
    char* fn = Filename.Str;
    TString NodeName;
    TString ParentNodeName;
    FILE *fp;
    fopen_s(&fp, fn, "rb");
    if (!fp)
        return false;

    char magic[8];
    fread(&magic, sizeof(char) * 8, 1, fp);
    int version;
    fread(&version, sizeof(int), 1, fp);
    int designerId;
    fread(&designerId, sizeof(int), 1, fp);
    int numBones;
    fread(&numBones, sizeof(int), 1, fp);

    BoneHierarchy = new RBoneHierarchy();

    for (int i = 0; i < numBones; ++i) {
        char name[32];
        fread(&name, sizeof(char) * 32, 1, fp);
        int parentId;
        fread(&parentId, sizeof(int), 1, fp);
        float scale;
        fread(&scale, sizeof(float), 1, fp);

        RBone* bone = new RBone();
        bone->BoneName = name;
		bone->ParentIndex = parentId;
        if (parentId != -1) {
            bone->Parent = BoneHierarchy->Bones(parentId);
        } else {
            bone->Parent = 0;
        }
        TMatrix TM;
        fread(&TM._11, sizeof(float), 1, fp);
        fread(&TM._21, sizeof(float), 1, fp);
        fread(&TM._31, sizeof(float), 1, fp);
        fread(&TM._41, sizeof(float), 1, fp);
        fread(&TM._12, sizeof(float), 1, fp);
        fread(&TM._22, sizeof(float), 1, fp);
        fread(&TM._32, sizeof(float), 1, fp);
        fread(&TM._42, sizeof(float), 1, fp);
        fread(&TM._13, sizeof(float), 1, fp);
        fread(&TM._23, sizeof(float), 1, fp);
        fread(&TM._33, sizeof(float), 1, fp);
        fread(&TM._43, sizeof(float), 1, fp);
        TM._14 = 0;
        TM._24 = 0;
        TM._34 = 0;
        TM._44 = 1;
        bone->TM = TM;
        bone->InvTM = TM.Inverse();
        bone->BoneIndex = i;
        BoneHierarchy->Bones.AddItem(bone);
    }

    int boneIndices;
    fread(&boneIndices, sizeof(int), 1, fp);
    for (int i = 0; i < boneIndices; ++i) {
        fread(&BoneHierarchy->Bones[i]->SkinBoneIndex, sizeof(int), 1, fp);
    }

    fclose(fp);
	return true;
}
