#include "StdAfx.h"
#include "CSkeletalMeshPrimitive.h"

TSkeletalSubMeshPrimitive::TSkeletalSubMeshPrimitive() 
: pSubMesh(0), pMaterial(0), pBone(0), pAnimationSequence(0)
{

}


void TSkeletalSubMeshPrimitive::Render(TBatch *Batch)
{
	RSubMesh *Mesh = pSubMesh;
	Batch->nVertexStride = Mesh->pVB->nVertexStride;
	Batch->nVertices += Mesh->pVB->nVertices;
	Batch->nIndices += Mesh->pIB->nIndices;
}

unsigned int TSkeletalSubMeshPrimitive::FillDynamicVertexBuffer(char** pData)
{
	memcpy(pData, pSubMesh->pVB->pVertices, 
		pSubMesh->pVB->nVertices * pSubMesh->pVB->nVertexStride);
	for(int k=0;k<pSubMesh->pVB->nVertices;++k)
	{
		*((TVector3*)&((*pData)[k*pSubMesh->pVB->nVertexStride])) = TM.TransformVector3(*((TVector3*)&((*pData)[k*pSubMesh->pVB->nVertexStride])));
	}
	*pData += pSubMesh->pVB->nVertices * pSubMesh->pVB->nVertexStride;
	return pSubMesh->pVB->nVertices;
}

unsigned int TSkeletalSubMeshPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex)
{
	for(int k=0;k<pSubMesh->pIB->nIndices;++k)
	{
		TIndex16 tmpIndex;
		tmpIndex._1 = pSubMesh->pIB->pIndices[k]._1 + *BaseIndex;
		tmpIndex._2 = pSubMesh->pIB->pIndices[k]._2 + *BaseIndex;
		tmpIndex._3 = pSubMesh->pIB->pIndices[k]._3 + *BaseIndex;
		(*pData)[k] = tmpIndex;
	}
	*BaseIndex += pSubMesh->pVB->nVertices;
	*pData += pSubMesh->pIB->nIndices;

	return pSubMesh->pVB->nVertices;
}


CSkeletalMeshPrimitive::CSkeletalMeshPrimitive(RBoneInfo* BoneInfo)
{
	if(BoneInfo)
	{
		for(unsigned int i=0;i<BoneInfo->Bones.Size();++i)
		{
			RBone* RBoneSrc = BoneInfo->Bones(i);
			TSkeletalBone* TBoneRef = new TSkeletalBone();
			CopyRBoneToTSkeletalBone(RBoneSrc, TBoneRef);
			BoneRef.AddItem(TBoneRef);
		}
	}
}

CSkeletalMeshPrimitive::~CSkeletalMeshPrimitive(void)
{
	for(unsigned int i=0;i<BoneRef.Size();++i)
	{
		delete BoneRef(i);
	}
	BoneRef.Clear();
}

void CSkeletalMeshPrimitive::CopyRBoneToTSkeletalBone(RBone *Bone, TSkeletalBone *TBone)
{
	strcpy_s(TBone->BoneName.Str, 1024, Bone->BoneName.Str);
	for(unsigned int i=0;i<BoneRef.Size();++i)
	{
		if(!strcmp(BoneRef(i)->BoneName.Str, Bone->Parent->BoneName.Str))
		{
			TBone->Parent = BoneRef(i);
		}
	}
	TBone->Rotation = Bone->Rotation;
	TBone->Scale = Bone->Scale;
	TBone->Translation = Bone->Translation;
}