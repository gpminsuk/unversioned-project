#pragma once

#include "CMeshPrimitive.h"

#include "RResource.h"

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

	TSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence = NULL);
	~TSkeletalMesh();

	class TBone
	{
	public:
		TBone(RBoneHierarchy::RBone* InBone, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence = NULL);
		~TBone();

		RBoneHierarchy::RBone* BoneRef;
		TArray<RSkeletalSubMesh*> SubMesheRefs;
		RAnimationBoneSequence* AnimationBoneSequenceRef;

		TArray<TBone*> ChildBones;

		void FillStaticVertexBuffer_Recursive(VD* pVertices, TMatrix BoneTM = TMatrix());
		void FillStaticIndexBuffer_Recursive(ID* pIndices);

		unsigned int NumTotalVertices_Recursive();
		unsigned int NumTotalIndices_Recursive();
	};
	TBone* RootBone;
};

class CSkeletalMeshPrimitive : public CMeshPrimitive
{
public:
	CSkeletalMeshPrimitive(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence);
	~CSkeletalMeshPrimitive(void);

	TSkeletalMesh* SkeletalMeshTemplate;

	virtual void Render(TBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};
