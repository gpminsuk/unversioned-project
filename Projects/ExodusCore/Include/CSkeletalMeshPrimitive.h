#pragma once

#include "CMeshPrimitive.h"

#include "RAnimationSequence.h"
#include "RBoneHierarchy.h"
#include "RMesh.h"

class CSkeletalMeshDraw: public BDraw
{
public:
	typedef TIndex16 ID;

	CSkeletalMeshDraw(RMesh* InSkeletalMesh, RBoneHierarchy* InBoneHierarchy);
	~CSkeletalMeshDraw();

	class TBone
	{
	public:
		TBone(RBone* InSource, TBone* InParent, RAnimationBoneSequence* InAnimationBoneSequence);
		~TBone();

		TMatrix BoneTM;

		RBone* Source;
		TBone* Parent;
		TArray<RSubMesh*> SubMesheRefs;
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
	RAnimationSequence* AnimationSequenceRef;
	RMesh* Mesh;
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

	void SetSkeletalMesh(RMesh* InMesh, RBoneHierarchy* InBoneHierarchy);

	virtual void CreateDraws();
	virtual bool Access(AAccessor& Accessor);
	virtual void UpdatePrimitive();
	virtual RMaterial* GetMaterial();
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();

};
