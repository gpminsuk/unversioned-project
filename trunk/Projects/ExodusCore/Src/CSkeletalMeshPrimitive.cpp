#include "StdAfx.h"
#include "CSkeletalMeshPrimitive.h"
#include "BRenderingBatch.h"
#include "BDriver.h"

IMPLEMENT_CLASS(CSkeletalMeshPrimitive)

CSkeletalMeshPrimitive::CSkeletalMeshPrimitive() {
    RenderType = RenderType_Opaque;
}

CSkeletalMeshPrimitive::~CSkeletalMeshPrimitive(void) {
    for (unsigned int i = 0; i < Draws.Size(); ++i) {
        delete Draws(i);
    }
}

void CSkeletalMeshPrimitive::SetSkeletalMesh(RMesh* InMesh, RBoneHierarchy* InBoneHierarchy) {
	Mesh.Set(InMesh);
	BoneHierarchy.Set(InBoneHierarchy);
}

bool CSkeletalMeshPrimitive::Access(AAccessor& Accessor) {
	__super::Access(Accessor);
	Accessor << Mesh;
	Accessor << BoneHierarchy;
	if(Accessor.IsLoading()) {
		CreateDraws();
	}
	return true;
}

void CSkeletalMeshPrimitive::CreateDraws() {
	Draws.AddItem(new CSkeletalMeshDraw(Mesh.Get(), BoneHierarchy.Get()));
}

void CSkeletalMeshPrimitive::UpdatePrimitive() {
    NumIndices = 0;
    for (unsigned int i = 0; i < Draws.Size(); ++i) {
        Draws(i)->UpdatePrimitive();
        NumIndices += Draws(i)->pBuffer->m_pIB->nIndices;
    }
}

RMaterial* CSkeletalMeshPrimitive::GetMaterial() {
	return RMaterialTable::Materials(0);
}

unsigned int CSkeletalMeshPrimitive::FillDynamicVertexBuffer(char** pData) {
	GDriver->SetTexture(0, GDefaultTexture);

	unsigned int nVerticies = 0;
	for (unsigned int i = 0; i < Draws.Size(); ++i) {
		BDraw* Primitive = Draws(i);
		memcpy((*pData), Primitive->pBuffer->m_pVB->pVertices,
			Primitive->pBuffer->m_pVB->nVertices
			* Primitive->pBuffer->m_pVB->Protocol->Decl->GetStride());
		RVertexDeclaration::Position_Normal_TexCoord_VD* pVertices = reinterpret_cast<RVertexDeclaration::Position_Normal_TexCoord_VD*>(*pData);
		for (unsigned int k = 0; k < Primitive->pBuffer->m_pVB->nVertices; ++k) {			
			pVertices[k].Position = TM.TransformVector3(pVertices[k].Position);
		}
		*pData += Primitive->pBuffer->m_pVB->nVertices
			* Primitive->pBuffer->m_pVB->Protocol->Decl->GetStride();
		nVerticies += Primitive->pBuffer->m_pVB->nVertices;
	}
	return nVerticies;
}

unsigned int CSkeletalMeshPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) {
	for (unsigned int i = 0; i < Draws.Size(); ++i) {
		BDraw* Primitive = Draws(i);
		for (unsigned int k = 0; k < Primitive->pBuffer->m_pIB->nIndices; ++k) {
			TIndex16 tmpIndex;
			tmpIndex._1 = Primitive->pBuffer->m_pIB->pIndices[k]._1
				+ *BaseIndex;
			tmpIndex._2 = Primitive->pBuffer->m_pIB->pIndices[k]._2
				+ *BaseIndex;
			tmpIndex._3 = Primitive->pBuffer->m_pIB->pIndices[k]._3
				+ *BaseIndex;
			(*pData)[k] = tmpIndex;
		}
		*BaseIndex += Primitive->pBuffer->m_pVB->nVertices;
		*pData += Primitive->pBuffer->m_pIB->nIndices;
	}
	return 0;
}

unsigned int CSkeletalMeshPrimitive::GetNumIndices() {
	return NumIndices;
}

CSkeletalMeshDraw::CSkeletalMeshDraw(RMesh* InMesh, RBoneHierarchy* InBoneHierarchy)
	:
	Mesh(InMesh),
	CurrentFrame(0),
	AnimationSequenceRef(0) {
		if (InBoneHierarchy) {
			for (unsigned int i = 0; i < InBoneHierarchy->Bones.Size(); ++i) {
				TBone* Parent = 0;
				if (InBoneHierarchy->Bones[i]->ParentIndex != -1) {
					Parent = Bones[InBoneHierarchy->Bones[InBoneHierarchy->Bones[i]->ParentIndex]->BoneIndex];
				}
				Bones.AddItem(
					new TBone(InBoneHierarchy->Bones[i], Parent,
					0));
				SkinBoneIndexMap.AddItem(InBoneHierarchy->Bones[i]->SkinBoneIndex);
			}
		}

		pBuffer = new RStaticPrimitiveBuffer();

		RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
		RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
		pBuffer->m_pVB = pVB;
		pBuffer->m_pIB = pIB;

		pVB->Protocol = RVertexProtocol::Protocols(0);//InSkeletalMesh->SkeletalSubMeshes(0)->pVB->Declaration;
		pVB->nVertices = InMesh->SkeletalSubMeshes(0)->pVB->nVertices;
		pVB->pVertices = new char[pVB->Protocol->Decl->GetStride()* pVB->nVertices];
		RVertexDeclaration::Position_Normal_TexCoord_VD* pVertices = reinterpret_cast<RVertexDeclaration::Position_Normal_TexCoord_VD*>(pVB->pVertices);
		RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pSrcVertices = reinterpret_cast<RVertexDeclaration::SkeletalMesh_GPU_Skin_VD*>(InMesh->SkeletalSubMeshes(0)->pVB->pVertices);
		for(unsigned int i=0;i<pVB->nVertices;++i) {
			pVertices[i].Position = pSrcVertices[i].Position;
			pVertices[i].Normal = pSrcVertices[i].Normal;
			pVertices[i].TexCoord = pSrcVertices[i].TexCoord;
		}

		pIB->nIndices = InMesh->SkeletalSubMeshes(0)->pIB->nIndices;
		pIB->pIndices = new ID[pIB->nIndices];
		memcpy_s(pIB->pIndices, pIB->nIndices * sizeof(ID),
			InMesh->SkeletalSubMeshes(0)->pIB->pIndices,
			pIB->nIndices * sizeof(ID));

		UpdatePrimitive();
}

CSkeletalMeshDraw::~CSkeletalMeshDraw() {
    for (unsigned int i = 0; i < Bones.Size(); ++i) {
        delete Bones[i];
    }
    Bones.Clear();
}

void CSkeletalMeshDraw::UpdatePrimitive() {
    if (IsPlaying) {
        //CurrentFrame ++;
    }

    if (AnimationSequenceRef
            && 50 < CurrentFrame
            && IsLooping) {
        CurrentFrame = 0;
    }

    CalcBoneMatrices();

    RSystemMemoryVertexBuffer *pVB = pBuffer->m_pVB;
    RSystemMemoryIndexBuffer *pIB = pBuffer->m_pIB;

    RVertexDeclaration::Position_Normal_TexCoord_VD* pVertex = reinterpret_cast<RVertexDeclaration::Position_Normal_TexCoord_VD*>(pVB->pVertices);
    RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pSrcVertex = reinterpret_cast<RVertexDeclaration::SkeletalMesh_GPU_Skin_VD*>(Mesh->SkeletalSubMeshes(0)->pVB
                                           ->pVertices);

    if (Bones.Size() > 0) {
        for (unsigned int i = 0; i < pVB->nVertices; ++i) {
            TMatrix BoneTM(0);
            for (int j = 0; j < 4; ++j) {
                BoneTM += (Bones(SkinBoneIndexMap(pSrcVertex[i].BoneIndices[j]))
                           ->Source->InvTM * pSrcVertex[i].BoneWeights[j]);
            }
            TMatrix WeightedMatrix(0);
            for (int j = 0; j < 4; ++j) {
                WeightedMatrix += (Bones(
                                       SkinBoneIndexMap(pSrcVertex[i].BoneIndices[j]))->BoneTM
                                   * pSrcVertex[i].BoneWeights[j]);
            }
            BoneTM *= WeightedMatrix;
            pVertex[i].Position = BoneTM.TransformVector3(pSrcVertex[i].Position);
			pVertex[i].Normal = BoneTM.TransformVector3(pSrcVertex[i].Position + pSrcVertex[i].Normal);
			pVertex[i].Normal -= pVertex[i].Position;
			pVertex[i].Normal.Normalize();
        }
    }
}

void CSkeletalMeshDraw::CalcBoneMatrices() {
    for (unsigned int i = 0; i < Bones.Size(); ++i) {
        Bones(i)->CalcBoneMatrix(CurrentFrame);
    }
}

void CSkeletalMeshDraw::TBone::CalcBoneMatrix(unsigned int CurrentFrame) {
	BoneTM = Source->TM;
	if (AnimationBoneSequenceRef) {
		BoneTM.SetIdentity();
        TQuaternion Quat = AnimationBoneSequenceRef->GetRotKey(CurrentFrame);
        BoneTM.Rotate(Quat);
        TVector3 Pos = AnimationBoneSequenceRef->GetPosKey(CurrentFrame);
        BoneTM.Translate(Pos);
		if (Parent) {
			BoneTM *= Parent->BoneTM;
		}
    }    
}

CSkeletalMeshDraw::TBone::TBone(RBone* InSource, TBone* InParent, RAnimationBoneSequence* InAnimationBoneSequence)
    :
    Source(InSource),
    AnimationBoneSequenceRef(InAnimationBoneSequence),
    Parent(InParent) {
}

CSkeletalMeshDraw::TBone::~TBone() {
}

unsigned int CSkeletalMeshDraw::TBone::NumTotalVertices_Recursive() {
    unsigned int NumVertices = 0;
    return NumVertices;
}

unsigned int CSkeletalMeshDraw::TBone::NumTotalIndices_Recursive() {
    unsigned int NumIndices = 0;
    return NumIndices;
}

char* CSkeletalMeshDraw::TBone::FillStaticVertexBuffer_Recursive(char* pVertices) {
    return pVertices;
}

CSkeletalMeshDraw::ID* CSkeletalMeshDraw::TBone::FillStaticIndexBuffer_Recursive(ID* pIndices, unsigned short* BaseIndex) {
    return pIndices;
}
