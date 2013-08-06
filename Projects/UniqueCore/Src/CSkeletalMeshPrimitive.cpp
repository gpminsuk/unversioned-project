#include "StdAfx.h"
#include "CSkeletalMeshPrimitive.h"
#include "BRenderingBatch.h"
#include "BDriver.h"

#define TEST_BIP_EXCLUDED

CSkeletalMeshPrimitive::CSkeletalMeshPrimitive() {
    RenderType = RenderType_Opaque;

    extern RTextureBuffer* GDefaultTexture;
    Texture = GDefaultTexture;
}

CSkeletalMeshPrimitive::~CSkeletalMeshPrimitive(void) {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        delete Primitives(i);
    }
}

void CSkeletalMeshPrimitive::SetSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* AnimationSeq) {
    SkeletalMeshTemplate = new TSkeletalMesh(InBoneHierarchy, InSkeletalMesh,
            AnimationSeq);
    UpdatePrimitive();
    Primitives.AddItem(SkeletalMeshTemplate);
}

void CSkeletalMeshPrimitive::UpdatePrimitive() {
    NumIndices = 0;
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        Primitives(i)->UpdatePrimitive();
        NumIndices += Primitives(i)->pBuffer->m_pIB->nIndices;
    }
}

RShaderBase* CSkeletalMeshPrimitive::GetShaderType() {
	return RShaderTable::Shaders(0);
}

unsigned int CSkeletalMeshPrimitive::FillDynamicVertexBuffer(char** pData) {
    GDriver->SetTexture(0, Texture);

    unsigned int nVerticies = 0;
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        TPrimitive* Primitive = Primitives(i);
        memcpy((*pData), Primitive->pBuffer->m_pVB->pVertices,
               Primitive->pBuffer->m_pVB->nVertices
               * Primitive->pBuffer->m_pVB->Declaration->GetStride());
        for (unsigned int k = 0; k < Primitive->pBuffer->m_pVB->nVertices; ++k) {
            *((TVector3*) &((*pData)[k
                                     * Primitive->pBuffer->m_pVB->Declaration->GetStride()])) = TM
                                             .TransformVector3(
                                                 *((TVector3*) &((*pData)[k
                                                         * Primitive->pBuffer->m_pVB->Declaration->GetStride()])));
        }
        *pData += Primitive->pBuffer->m_pVB->nVertices
                  * Primitive->pBuffer->m_pVB->Declaration->GetStride();
        nVerticies += Primitive->pBuffer->m_pVB->nVertices;
    }
    return nVerticies;
}

unsigned int CSkeletalMeshPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        TPrimitive* Primitive = Primitives(i);
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

TSkeletalMesh::TSkeletalMesh(RBoneHierarchy* InBoneHierarchy, RSkeletalMesh* InSkeletalMesh, RAnimationSequence* InAnimationSequence)
    :
    CurrentFrame(0),
    AnimationSequenceRef(InAnimationSequence),
    Source(InSkeletalMesh) {
    if (InBoneHierarchy) {
        for (unsigned int i = 0; i < InBoneHierarchy->Bones.Size(); ++i) {
            TBone* Parent = 0;
            if (InBoneHierarchy->Bones[i]->Parent != 0) {
                Parent = Bones[InBoneHierarchy->Bones[i]->Parent->BoneIndex];
            }
            Bones.AddItem(
                new TBone(InBoneHierarchy->Bones[i], Parent,
                          InBoneHierarchy->Bones[i]->AnimationBoneSequenceRef));
            SkinBoneIndexMap.AddItem(InBoneHierarchy->Bones[i]->SkinBoneIndex);
        }
    }

    pBuffer = new RStaticPrimitiveBuffer();

    RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
    RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
    pBuffer->m_pVB = pVB;
    pBuffer->m_pIB = pIB;
	
    pVB->Declaration = RVertexDeclaration::Position_Normal_TexCoord;//InSkeletalMesh->SkeletalSubMeshes(0)->pVB->Declaration;
    pVB->nVertices = InSkeletalMesh->SkeletalSubMeshes(0)->pVB->nVertices;
    pVB->pVertices = new char[pVB->Declaration->GetStride()* pVB->nVertices];
	RVertexDeclaration::Position_Normal_TexCoord_VD* pVertices = reinterpret_cast<RVertexDeclaration::Position_Normal_TexCoord_VD*>(pVB->pVertices);
	RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pSrcVertices = reinterpret_cast<RVertexDeclaration::SkeletalMesh_GPU_Skin_VD*>(InSkeletalMesh->SkeletalSubMeshes(0)->pVB->pVertices);
	for(unsigned int i=0;i<pVB->nVertices;++i) {
		pVertices[i].Position = pSrcVertices[i].Position;
		pVertices[i].Normal = pSrcVertices[i].Normal;
		pVertices[i].TexCoord = pSrcVertices[i].TexCoord;
	}
    memcpy_s(pVB->pVertices, pVB->Declaration->GetStride()* pVB->nVertices,
             InSkeletalMesh->SkeletalSubMeshes(0)->pVB->pVertices,
             pVB->Declaration->GetStride() * pVB->nVertices);

    pIB->nIndices = InSkeletalMesh->SkeletalSubMeshes(0)->pIB->nIndices;
    pIB->pIndices = new ID[pIB->nIndices];
    memcpy_s(pIB->pIndices, pIB->nIndices * sizeof(ID),
             InSkeletalMesh->SkeletalSubMeshes(0)->pIB->pIndices,
             pIB->nIndices * sizeof(ID));

    UpdatePrimitive();
}

TSkeletalMesh::~TSkeletalMesh() {
    for (unsigned int i = 0; i < Bones.Size(); ++i) {
        delete Bones[i];
    }
    Bones.Clear();
}

void TSkeletalMesh::UpdatePrimitive() {
    if (IsPlaying) {
        CurrentFrame ++;
    }

    if (AnimationSequenceRef
            && 100 < CurrentFrame
            && IsLooping) {
        CurrentFrame = 0;
    }

    CalcBoneMatrices();

    RSystemMemoryVertexBuffer *pVB = pBuffer->m_pVB;
    RSystemMemoryIndexBuffer *pIB = pBuffer->m_pIB;

    RVertexDeclaration::Position_Normal_TexCoord_VD* pVertex = reinterpret_cast<RVertexDeclaration::Position_Normal_TexCoord_VD*>(pVB->pVertices);
    RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pSrcVertex = reinterpret_cast<RVertexDeclaration::SkeletalMesh_GPU_Skin_VD*>(Source->SkeletalSubMeshes(0)->pVB
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
			BoneTM.Translate(TVector3(0,0,0));
			pVertex[i].Normal = BoneTM.TransformVector3(pSrcVertex[i].Normal);
        }
    }
}

void TSkeletalMesh::CalcBoneMatrices() {
    for (unsigned int i = 0; i < Bones.Size(); ++i) {
        Bones(i)->CalcBoneMatrix(CurrentFrame);
    }
}

void TSkeletalMesh::TBone::CalcBoneMatrix(unsigned int CurrentFrame) {
    BoneTM.SetIdentity();
    if (AnimationBoneSequenceRef) {
        TQuaternion Quat = AnimationBoneSequenceRef->GetRotKey(CurrentFrame);
        BoneTM.Rotate(Quat);
        TVector3 Pos = AnimationBoneSequenceRef->GetPosKey(CurrentFrame);
        BoneTM.Translate(Pos);
    }
    if (Parent) {
        BoneTM *= Parent->BoneTM;
    }
}

TSkeletalMesh::TBone::TBone(RBone* InSource, TBone* InParent, RAnimationBoneSequence* InAnimationBoneSequence)
    :
    Source(InSource),
    AnimationBoneSequenceRef(InAnimationBoneSequence),
    Parent(InParent) {
}

TSkeletalMesh::TBone::~TBone() {
}

unsigned int TSkeletalMesh::TBone::NumTotalVertices_Recursive() {
    unsigned int NumVertices = 0;
    return NumVertices;
}

unsigned int TSkeletalMesh::TBone::NumTotalIndices_Recursive() {
    unsigned int NumIndices = 0;
    return NumIndices;
}

char* TSkeletalMesh::TBone::FillStaticVertexBuffer_Recursive(char* pVertices) {
    return pVertices;
}

TSkeletalMesh::ID* TSkeletalMesh::TBone::FillStaticIndexBuffer_Recursive(ID* pIndices, unsigned short* BaseIndex) {
    return pIndices;
}
