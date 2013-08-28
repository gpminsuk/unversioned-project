#pragma once

#include "CMeshPrimitive.h"

#include "RAnimationSequence.h"
#include "RBoneHierarchy.h"
#include "RAnimationTree.h"
#include "RMesh.h"

class TBone
{
public:
	TBone(RBone* InSource, TBone* InParent);
	~TBone();
	
	RBone* Source;
	TBone* Parent;

	TArray<TQuaternion> WeightedRot;
	TArray<TVector3> WeightedPos;
	TArray<float> Weights;
	TMatrix TM;

	void CalcBoneMatrix(RAnimationBoneSequence* AnimationBoneSequence, unsigned int CurrentFrame, float Weight);
};

class CSkeletalMeshDraw: public BDraw
{
public:
	typedef TIndex16 ID;

	CSkeletalMeshDraw(RMesh* InMesh);
	~CSkeletalMeshDraw();

	unsigned int CurrentFrame;
	bool IsPlaying;
	bool IsLooping;
};

class CSkeletalMeshPrimitive: public BPrimitive
{
	DECLARE_CLASS(CSkeletalMeshPrimitive,)
public:
	CSkeletalMeshPrimitive();
	~CSkeletalMeshPrimitive(void);

	RAssetPtr<RMesh> Mesh;
	RAssetPtr<RBoneHierarchy> BoneHierarchy;
	RAssetPtr<RAnimationTree> AnimationTree;

	TArray<TBone*> Bones;

	void SetSkeletalMesh(RMesh* InMesh, RBoneHierarchy* InBoneHierarchy);
	void SetAnimationTree(RAnimationTree* InAnimationTree);

	virtual void CreateDraws();
	virtual bool Access(AAccessor& Accessor);
	virtual void UpdatePrimitive();
	virtual RMaterial* GetMaterial();
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
	void CalcBoneMatrices();

	virtual RTexture* GetTexture();
	virtual TArray<TString> GetNeededAssetNames() {
		TArray<TString> Ret;
		Ret.AddItem(TString("RMesh"));
		Ret.AddItem(TString("RBoneHierarchy"));
		return Ret;
	}

	virtual TArray<TString> GetCreatableThingClassNames() {
		TArray<TString> Ret;
		Ret.AddItem(TString("CCharacter"));
		return Ret;
	}
};
