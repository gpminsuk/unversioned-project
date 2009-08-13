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

class TSkeletalSubMeshPrimitive : public TPrimitive
{
public:
	TSkeletalSubMeshPrimitive();
	~TSkeletalSubMeshPrimitive();

	virtual void Render(TBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);

	class RMaterial *pMaterial;

	class RSubMesh* pSubMesh;

	class RAnimationBoneSequence *pAnimationSequence;
	class RBone				 *pBone;
};

class TSkeletalMeshPrimitive : public TPrimitive
{
public:
	TSkeletalMeshPrimitive();
	~TSkeletalMeshPrimitive();

	virtual void Render(TBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);

	TArray<TSkeletalSubMeshPrimitive*>	SubMeshes;
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
