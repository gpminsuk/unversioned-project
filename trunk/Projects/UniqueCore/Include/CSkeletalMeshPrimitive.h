#pragma once

#include "CMeshPrimitive.h"

#include "RAnimationSequence.h"
#include "RBoneHierarchy.h"
#include "RSkeletalMesh.h"

class TSkeletalMesh : public TPrimitive
{
public:
	typedef struct
	{
		TVector3 Pos;
		TVector3 Normal;
		TVector2 UV;
	} VD;

	typedef TIndex16 ID;

	TSkeletalMesh(RBone* InBone, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence = NULL);
	~TSkeletalMesh();

	class TBone
	{
	public:
		TBone(RBone* InBone, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence = NULL);
		~TBone();

		TMatrix BoneTM;

		RBone* BoneRef;
		TArray<RSkeletalSubMesh*> SubMesheRefs;
		RAnimationBoneSequence* AnimationBoneSequenceRef;

		TArray<TBone*> ChildBones;

		VD* FillStaticVertexBuffer_Recursive(VD* pVertices);
		ID* FillStaticIndexBuffer_Recursive(ID* pIndices, unsigned short* BaseIndex);

		unsigned int NumTotalVertices_Recursive();
		unsigned int NumTotalIndices_Recursive();

		void CalcBoneMatrices_Recursive(unsigned int CurrentFrame, const TMatrix& ParentTM = TMatrix::Identity);
	};

	virtual void UpdatePrimitive();
	virtual void CalcBoneMatrices();

	TBone* RootBone;
	RAnimationSequence* AnimationSequenceRef;
	unsigned int CurrentFrame;
	bool IsPlaying;
	bool IsLooping;
};

class CSkeletalMeshPrimitive : public CMeshPrimitive
{
public:
	CSkeletalMeshPrimitive();
	~CSkeletalMeshPrimitive(void);

	TSkeletalMesh* SkeletalMeshTemplate;
	RTextureBuffer* Texture;

	void SetSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* AnimationSeq);

	virtual void UpdatePrimitive();

	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};
