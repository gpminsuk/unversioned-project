#pragma once

#include "CMeshPrimitive.h"

#include "RAnimationSequence.h"
#include "RBoneHierarchy.h"
#include "RSkeletalMesh.h"

class TSkeletalMesh: public TPrimitive
{
public:
	typedef TIndex16 ID;

	TSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence =
			NULL);
	~TSkeletalMesh();

	class TBone
	{
	public:
		TBone(RBone* InSource, TBone* InParent, RAnimationBoneSequence* InAnimationBoneSequence);
		~TBone();

		TMatrix BoneTM;

		RBone* Source;
		TBone* Parent;
		TArray<RSkeletalSubMesh*> SubMesheRefs;
		RAnimationBoneSequence* AnimationBoneSequenceRef;

		char* FillStaticVertexBuffer_Recursive(char* pVertices);
		ID* FillStaticIndexBuffer_Recursive(ID* pIndices, unsigned short* BaseIndex);

		unsigned int NumTotalVertices_Recursive();
		unsigned int NumTotalIndices_Recursive();

		void CalcBoneMatrix(unsigned int CurrentFrame);
	};

	virtual void UpdatePrimitive();
	virtual void CalcBoneMatrices();

	TArray<TBone*> Bones;
	TArray<int> SkinBoneIndexMap;
	RSkeletalMesh* Source;
	RAnimationSequence* AnimationSequenceRef;
	unsigned int CurrentFrame;
	bool IsPlaying;
	bool IsLooping;
};

class CSkeletalMeshPrimitive: public CMeshPrimitive
{
public:
	CSkeletalMeshPrimitive();
	~CSkeletalMeshPrimitive(void);

	TSkeletalMesh* SkeletalMeshTemplate;
	RTextureBuffer* Texture;

	void SetSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* AnimationSeq);

	virtual void UpdatePrimitive();

	virtual RShaderBase* GetShaderType();
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};
