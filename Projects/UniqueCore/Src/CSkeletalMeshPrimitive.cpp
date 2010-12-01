#include "StdAfx.h"
#include "CSkeletalMeshPrimitive.h"
#include "BRenderingBatch.h"

#define TEST_BIP_EXCLUDED

CSkeletalMeshPrimitive::CSkeletalMeshPrimitive(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence)
{
	RenderType = RenderType_Opaque;

	SkeletalMeshTemplate = new TSkeletalMesh(InBoneHierarchy, InSkeletalMesh, InAnimationSequence);
	UpdatePrimitive();
	Primitives.AddItem(SkeletalMeshTemplate);
}

CSkeletalMeshPrimitive::~CSkeletalMeshPrimitive(void)
{
	delete SkeletalMeshTemplate;
}

void CSkeletalMeshPrimitive::UpdatePrimitive()
{
	NumIndices = 0;
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		Primitives(i)->UpdatePrimitive();
		NumIndices += Primitives(i)->pBuffer->m_pIB->nIndices;
	}
}

unsigned int CSkeletalMeshPrimitive::FillDynamicVertexBuffer(char** pData)
{
	UpdatePrimitive();
	TPrimitive* Primitive = Primitives(0);
	memcpy((*pData), Primitive->pBuffer->m_pVB->pVertices, 
		Primitive->pBuffer->m_pVB->nVertices * Primitive->pBuffer->m_pVB->nVertexStride);
	for(unsigned int k=0;k<Primitive->pBuffer->m_pVB->nVertices;++k)
	{
		*((TVector3*)&((*pData)[k*Primitive->pBuffer->m_pVB->nVertexStride])) = TM.TransformVector3(*((TVector3*)&((*pData)[k*Primitive->pBuffer->m_pVB->nVertexStride])));
	}
	*pData += Primitive->pBuffer->m_pVB->nVertices * Primitive->pBuffer->m_pVB->nVertexStride;
	return Primitive->pBuffer->m_pVB->nVertices;
}

unsigned int CSkeletalMeshPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex)
{
	TPrimitive* Primitive = Primitives(0);
	for(unsigned int k=0;k<GetNumIndices();++k)
	{
		TIndex16 tmpIndex;
		tmpIndex._1 = Primitive->pBuffer->m_pIB->pIndices[k]._1 + *BaseIndex;
		tmpIndex._2 = Primitive->pBuffer->m_pIB->pIndices[k]._2 + *BaseIndex;
		tmpIndex._3 = Primitive->pBuffer->m_pIB->pIndices[k]._3 + *BaseIndex;
		(*pData)[k] = tmpIndex;
	}
	*BaseIndex += Primitive->pBuffer->m_pVB->nVertices;
	*pData += GetNumIndices();

	return Primitive->pBuffer->m_pVB->nVertices;
}

unsigned int CSkeletalMeshPrimitive::GetNumIndices()
{
	return NumIndices;
}

TSkeletalMesh::TSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence)
:	CurrentFrame(0),
	AnimationSequenceRef(InAnimationSequence)
{
	RootBone = new TBone(InBoneHierarchy->RootBone, InSkeletalMesh, InAnimationSequence);	

	UpdatePrimitive();
}

TSkeletalMesh::~TSkeletalMesh()
{
	delete RootBone;
}

void TSkeletalMesh::UpdatePrimitive()
{
	delete pBuffer;

	CurrentFrame+=10;
	if(AnimationSequenceRef->EndFrame*AnimationSequenceRef->TickPerFrame < CurrentFrame)
	{
		CurrentFrame = 0;
	}

	CalcBoneMatrices();

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

	unsigned short BaseIndex = 0;
	RootBone->FillStaticIndexBuffer_Recursive(pIB->pIndices, &BaseIndex);
}

void TSkeletalMesh::CalcBoneMatrices()
{
	RootBone->CalcBoneMatrices_Recursive(CurrentFrame);
}

void TSkeletalMesh::TBone::CalcBoneMatrices_Recursive(unsigned int CurrentFrame, const TMatrix& ParentTM)
{	
	BoneTM.SetIdentity();
	if(AnimationBoneSequenceRef)
	{
		static int frame = 500;
		TQuaternion Quat = AnimationBoneSequenceRef->GetRotKey(CurrentFrame);
		if(Quat != TQuaternion())
		{
			BoneTM.Rotate(Quat);
		}
		else
		{
			BoneTM = BoneRef->TM;
		}
		TVector3 Pos = AnimationBoneSequenceRef->GetPosKey(CurrentFrame);
		if(Pos != TVector3(0,0,0))
		{
			BoneTM._41 = Pos.x;
			BoneTM._42 = Pos.y;
			BoneTM._43 = Pos.z;
		}
		else
		{
			BoneTM._41 = BoneRef->TM._41;
			BoneTM._42 = BoneRef->TM._42;
			BoneTM._43 = BoneRef->TM._43;
		}
	}
	else
	{
		BoneTM = BoneRef->TM;
	}
	BoneTM = BoneTM * ParentTM;
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		ChildBones(i)->CalcBoneMatrices_Recursive(CurrentFrame, BoneTM);
	}
}

TSkeletalMesh::TBone::TBone(RBoneHierarchy::RBone* InBone, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence)
:	AnimationBoneSequenceRef(0)
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
#endif
		NumIndices += SubMesh->pIB->nIndices;
	}
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		NumIndices += ChildBones(i)->NumTotalIndices_Recursive();
	}
	return NumIndices;
}

TSkeletalMesh::VD* TSkeletalMesh::TBone::FillStaticVertexBuffer_Recursive(VD* pVertices)
{
	for(unsigned int i=0;i<SubMesheRefs.Size();++i)
	{
		RSkeletalSubMesh* SubMesh = SubMesheRefs(i);
#ifdef TEST_BIP_EXCLUDED
		if(SubMesh->BoneName.Str[0] == 'B' && SubMesh->BoneName.Str[1] == 'i' && SubMesh->BoneName.Str[2] == 'p')
			continue;
#endif
		//if(!(SubMesh->BoneName.Str[0] == 'B' && SubMesh->BoneName.Str[1] == 'i' && SubMesh->BoneName.Str[2] == 'p'))
		//	continue;
		//if(!(SubMesh->BoneName.Str[0] == 'p' && SubMesh->BoneName.Str[1] == 'o' && SubMesh->BoneName.Str[2] == 'n'))
		//	continue;

		for(unsigned int j=0;j<SubMesh->pVB->nVertices;++j)
		{
			RSkeletalSubMesh::VD *Vertex = reinterpret_cast<RSkeletalSubMesh::VD*>(SubMesh->pVB->pVertices);
			pVertices[j].Pos = BoneTM.TransformVector3(Vertex[j].Pos);
			//pVertices[j].Pos = Vertex[j].Pos;
			pVertices[j].UV = Vertex[j].UV;
			pVertices[j].Normal = Vertex[i].Normal;
		}
		pVertices += SubMesh->pVB->nVertices;
	}
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		pVertices = ChildBones(i)->FillStaticVertexBuffer_Recursive(pVertices);
	}
	return pVertices;
}

TSkeletalMesh::ID* TSkeletalMesh::TBone::FillStaticIndexBuffer_Recursive(ID* pIndices, unsigned short* BaseIndex)
{
	for(unsigned int i=0;i<SubMesheRefs.Size();++i)
	{
		RSkeletalSubMesh* SubMesh = SubMesheRefs(i);
#ifdef TEST_BIP_EXCLUDED
		if(SubMesh->BoneName.Str[0] == 'B' && SubMesh->BoneName.Str[1] == 'i' && SubMesh->BoneName.Str[2] == 'p')
			continue;
#endif
		for(unsigned int j=0;j<SubMesh->pIB->nIndices;++j)
		{
			RSkeletalSubMesh::ID *Index = reinterpret_cast<RSkeletalSubMesh::ID*>(SubMesh->pIB->pIndices);
			pIndices[j]._1 = Index[j]._1 + *BaseIndex;
			pIndices[j]._2 = Index[j]._2 + *BaseIndex;
			pIndices[j]._3 = Index[j]._3 + *BaseIndex;
		}
		pIndices += SubMesh->pIB->nIndices;
		*BaseIndex += SubMesh->pVB->nVertices;
	}
	for(unsigned int i=0;i<ChildBones.Size();++i)
	{
		pIndices = ChildBones(i)->FillStaticIndexBuffer_Recursive(pIndices, BaseIndex);
	}
	return pIndices;
}