#include "stdafx.h"
#include "RANMImporter.h"

#include "RAnimationSequence.h"
#include "RBoneHierarchy.h"

bool RANMImporter::Import(TString& Filename, RAnimationSequence*& AnimationSequence, RBoneHierarchy*& BoneHierarchy, RMesh*& Model) {
    char* fn = Filename.Str;
    TString NodeName;
    TString ParentNodeName;
    FILE *fp;
    fopen_s(&fp, fn, "rb");
    if (!fp)
        return false;

    AnimationSequence = new RAnimationSequence();

    char magic[8];
    fread(&magic, sizeof(char) * 8, 1, fp);
    int version;
    fread(&version, sizeof(int), 1, fp);
    if (version == 3) {
        int designerId;
        fread(&designerId, sizeof(int), 1, fp);
        int numBones;
        fread(&numBones, sizeof(int), 1, fp);
        int numFrames;
        fread(&numFrames, sizeof(int), 1, fp);
        float fps;
        fread(&fps, sizeof(float), 1, fp);
        if (fps < 0) {
            fps += 4294967296.0f;
        }

		for (int i = 0; i < numBones; ++i) {
			AnimationSequence->AnimationBoneSequences.AddItem(0);
		}

        for (int i = 0; i < numBones; ++i) {
            char name[32];
            fread(name, sizeof(char) * 32, 1, fp);
            int flag;
            fread(&flag, sizeof(int), 1, fp);
            RAnimationBoneSequence* BoneSeq = new RAnimationBoneSequence(
                AnimationSequence);
            BoneSeq->BoneName = name;
            BoneSeq->AnimSequenceRef = AnimationSequence;
            for (int j = 0; j < numFrames; ++j) {
                RAnimationBoneSequence::ROTKEY RotationKey;
                fread(&RotationKey,
                      sizeof(RAnimationBoneSequence::ROTKEY) - sizeof(float),
                      1, fp);
                RotationKey.Rot.w = -RotationKey.Rot.w;
                RotationKey.Time = j;
                BoneSeq->RotKeys.AddItem(RotationKey);

                RAnimationBoneSequence::POSKEY PositionKey;
                fread(&PositionKey,
                      sizeof(RAnimationBoneSequence::POSKEY) - sizeof(float),
                      1, fp);
                PositionKey.Time = j;
                BoneSeq->PosKeys.AddItem(PositionKey);
            }
            RBone* Bone = BoneHierarchy->FindBone(TString(name));
			AnimationSequence->AnimationBoneSequences(Bone->BoneIndex) = BoneSeq;
        }
    }
    fclose(fp);
	return true;
}
