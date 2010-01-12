#include "StdAfx.h"
#include "CSkeletalMeshPrimitive.h"

#define TEST_BIP_EXCLUDED

CSkeletalMeshPrimitive::CSkeletalMeshPrimitive(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence)
{
	RenderType = RenderType_Opaque;

	SkeletalMeshTemplate = new TSkeletalMesh(InBoneHierarchy, InSkeletalMesh, InAnimationSequence);

	Primitives.AddItem(SkeletalMeshTemplate);
}

CSkeletalMeshPrimitive::~CSkeletalMeshPrimitive(void)
{
	delete SkeletalMeshTemplate;
}

void CSkeletalMeshPrimitive::Render(TBatch *Batch)
{
	Batch->nVertexStride = Primitives(0)->pBuffer->m_pVB->nVertexStride;
	Batch->nVertices += Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CSkeletalMeshPrimitive::FillDynamicVertexBuffer(char** pData)
{
	memcpy((*pData), Primitives(0)->pBuffer->m_pVB->pVertices, 
		Primitives(0)->pBuffer->m_pVB->nVertices * Primitives(0)->pBuffer->m_pVB->nVertexStride);
	for(unsigned int k=0;k<Primitives(0)->pBuffer->m_pVB->nVertices;++k)
	{
		*((TVector3*)&((*pData)[k*Primitives(0)->pBuffer->m_pVB->nVertexStride])) = TM.TransformVector3(*((TVector3*)&((*pData)[k*Primitives(0)->pBuffer->m_pVB->nVertexStride])));
	}
	*pData += Primitives(0)->pBuffer->m_pVB->nVertices * Primitives(0)->pBuffer->m_pVB->nVertexStride;
	return Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CSkeletalMeshPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex)
{
	for(unsigned int k=0;k<GetNumIndices();++k)
	{
		TIndex16 tmpIndex;
		tmpIndex._1 = Primitives(0)->pBuffer->m_pIB->pIndices[k]._1 + *BaseIndex;
		tmpIndex._2 = Primitives(0)->pBuffer->m_pIB->pIndices[k]._2 + *BaseIndex;
		tmpIndex._3 = Primitives(0)->pBuffer->m_pIB->pIndices[k]._3 + *BaseIndex;
		(*pData)[k] = tmpIndex;
	}
	*BaseIndex += Primitives(0)->pBuffer->m_pVB->nVertices;
	*pData += GetNumIndices();

	return Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CSkeletalMeshPrimitive::GetNumIndices()
{
	return Primitives(0)->pBuffer->m_pIB->nIndices;
}

TSkeletalMesh::TSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence)
{
	RootBone = new TBone(InBoneHierarchy->RootBone, InSkeletalMesh);

	pBuffer = new RStaticPrimitiveBuffer();

	RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
	RSystemMemoryVertexBufferTable::VertexBuffers.AddItem(pVB);
	RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
	RSystemMemoryIndexBufferTable::IndexBuffers.AddItem(pIB);
	pBuffer->m_pVB = pVB;
	pBuffer->m_pIB = pIB;

	pVB->nVertexStride = sizeof(VD);
	pVB->nVertices = RootBone->NumTotalVertices_Recursive();
	pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

	VD *pVertex = reinterpret_cast<VD*>(pVB->pVertices);

	RootBone->FillStaticVertexBuffer_Recursive(pVertex);

	pIB->nIndices = RootBone->NumTotalIndices_Recursive();
	pIB->pIndices = new ID[pIB->nIndices];

	RootBone->FillStaticIndexBuffer_Recursive(pIB->pIndices);
}

TSkeletalMesh::~TSkeletalMesh()
{
	delete RootBone;
}

TSkeletalMesh::TBone::TBone(RBoneHierarchy::RBone* InBone, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence)
{
	BoneRef = InBone;
	for(unsigned int i=0;i<InSkeletalMesh->SkeletalSubMeshes.Size();++i)
	{	
		RSkeletalSubMesh* SubMesh = InSkeletalMesh->SkeletalSubMeshes(i);
		if(SubMesh->BoneName == InBone->BoneName)
		{
			SubMesheRefs.AddItem(SubMesh);
		}
	}
	if(InAnimationSequence)
	{
		for(unsigned int i=0;i<InAnimationSequence->AnimationBoneSequences.Size();++i)
		{	
			RAnimationBoneSequence* AnimSeq = InAnimationSequence->AnimationBoneSequences(i);
			if(AnimSeq->BoneName == InBone->BoneName)
			{
				AnimationBoneSequenceRef = AnimSeq;
			}
		}
	}			
	for(unsigned int i=0;i<InBone->ChildBones.Size();++i)
	{
		TBone* Bone = new TBone(InBone->ChildBones(i), InSkeletalMesh, InAnimationSequence);
		ChildBones.AddItem(Bone);
	}
}

TSkeletalMesh::TBone::~TBone()
{
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		delete ChildBones(i);
	}
}

unsigned int TSkeletalMesh::TBone::NumTotalVertices_Recursive()
{
	unsigned int NumVertices = 0;
	for(unsigned int i=0;i<SubMesheRefs.Size();++i)
	{
		RSkeletalSubMesh* SubMesh = SubMesheRefs(i);
#ifdef TEST_BIP_EXCLUDED
		if(SubMesh->BoneName.Str[0] == 'B' && SubMesh->BoneName.Str[1] == 'i' && SubMesh->BoneName.Str[2] == 'p')
			continue;

		if(!(SubMesh->BoneName.Str[0] == 'p' && SubMesh->BoneName.Str[1] == 'o' && SubMesh->BoneName.Str[2] == 'n'))
			continue;
#endif
		NumVertices += SubMesh->pVB->nVertices;
	}
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		NumVertices += ChildBones(i)->NumTotalVertices_Recursive();
	}
	return NumVertices;
}

unsigned int TSkeletalMesh::TBone::NumTotalIndices_Recursive()
{
	unsigned int NumIndices = 0;
	for(unsigned int i=0;i<SubMesheRefs.Size();++i)
	{
		RSkeletalSubMesh* SubMesh = SubMesheRefs(i);
#ifdef TEST_BIP_EXCLUDED
		if(SubMesh->BoneName.Str[0] == 'B' && SubMesh->BoneName.Str[1] == 'i' && SubMesh->BoneName.Str[2] == 'p')
			continue;

		if(!(SubMesh->BoneName.Str[0] == 'p' && SubMesh->BoneName.Str[1] == 'o' && SubMesh->BoneName.Str[2] == 'n'))
			continue;
#endif
		NumIndices += SubMesh->pIB->nIndices;
	}
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		NumIndices += ChildBones(i)->NumTotalIndices_Recursive();
	}
	return NumIndices;
}

void TSkeletalMesh::TBone::FillStaticVertexBuffer_Recursive(VD* pVertices, TMatrix BoneTM)
{
	for(unsigned int i=0;i<SubMesheRefs.Size();++i)
	{
		RSkeletalSubMesh* SubMesh = SubMesheRefs(i);
#ifdef TEST_BIP_EXCLUDED
		if(SubMesh->BoneName.Str[0] == 'B' && SubMesh->BoneName.Str[1] == 'i' && SubMesh->BoneName.Str[2] == 'p')
			continue;

		if(!(SubMesh->BoneName.Str[0] == 'p' && SubMesh->BoneName.Str[1] == 'o' && SubMesh->BoneName.Str[2] == 'n'))
			continue;
#endif
		for(unsigned int j=0;j<SubMesh->pVB->nVertices;++j)
		{
			RSkeletalSubMesh::VD *Vertex = reinterpret_cast<RSkeletalSubMesh::VD*>(SubMesh->pVB->pVertices);
			pVertices[j].Pos = BoneRef->BoneTM.TransformVector3(Vertex[j].Pos);
			pVertices[j].UV = Vertex[j].UV;
			pVertices[j].Normal = Vertex[i].Normal;
		}
		pVertices += SubMesh->pVB->nVertices;
	}
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		BoneTM *= BoneRef->BoneTM;
		ChildBones(i)->FillStaticVertexBuffer_Recursive(pVertices, BoneTM);
	}
}

void TSkeletalMesh::TBone::FillStaticIndexBuffer_Recursive(ID* pIndices)
{
	for(unsigned int i=0;i<SubMesheRefs.Size();++i)
	{
		RSkeletalSubMesh* SubMesh = SubMesheRefs(i);
#ifdef TEST_BIP_EXCLUDED
		if(SubMesh->BoneName.Str[0] == 'B' && SubMesh->BoneName.Str[1] == 'i' && SubMesh->BoneName.Str[2] == 'p')
			continue;

		if(!(SubMesh->BoneName.Str[0] == 'p' && SubMesh->BoneName.Str[1] == 'o' && SubMesh->BoneName.Str[2] == 'n'))
			continue;
#endif
		for(unsigned int j=0;j<SubMesh->pIB->nIndices;++j)
		{
			RSkeletalSubMesh::ID *Index = reinterpret_cast<RSkeletalSubMesh::ID*>(SubMesh->pIB->pIndices);
			pIndices[j] = Index[j];
		}
		pIndices += SubMesh->pIB->nIndices;
	}
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		ChildBones(i)->FillStaticIndexBuffer_Recursive(pIndices);
	}
}