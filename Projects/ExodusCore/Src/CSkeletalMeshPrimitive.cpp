#include "StdAfx.h"
#include "CSkeletalMeshPrimitive.h"
#include "BRenderingBatch.h"
#include "BDriver.h"

#include "RTexture.h"

IMPLEMENT_CLASS(CSkeletalMeshPrimitive)

CSkeletalMeshPrimitive::CSkeletalMeshPrimitive() {
    RenderType = RenderType_Opaque;
	Batch = 0;
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

void CSkeletalMeshPrimitive::SetAnimationTree(RAnimationTree* InAnimationTree) {
	AnimationTree.Set(InAnimationTree);
	if (BoneHierarchy.Get()) {
		for (unsigned int i = 0; i < BoneHierarchy.Get()->Bones.Size(); ++i) {
			TBone* Parent = 0;
			if (BoneHierarchy.Get()->Bones[i]->ParentIndex != -1) {
				Parent = Bones[BoneHierarchy.Get()->Bones[BoneHierarchy.Get()->Bones[i]->ParentIndex]->BoneIndex];
			}
			Bones.AddItem(
				new TBone(BoneHierarchy.Get()->Bones[i], Parent));
			//SkinBoneIndexMap.AddItem(InBoneHierarchy->Bones[i]->SkinBoneIndex);
		}
	}
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
	BDraw* Draw = new CSkeletalMeshDraw(Mesh.Get());
	Draws.AddItem(Draw);
}

void CSkeletalMeshPrimitive::UpdatePrimitive() {	
	NumIndices = 0;
	for (unsigned int k = 0; k < Draws.Size(); ++k) {
		NumIndices += Draws(k)->pBuffer->m_pIB->nIndices;
	}
	if(Batch) {
		Batch->isDirty = true;
	}

	CalcBoneMatrices();

	for(unsigned int k=0;k<Draws.Size();++k) {
		RSystemMemoryVertexBuffer *pVB = Draws(k)->pBuffer->m_pVB;
		RSystemMemoryIndexBuffer *pIB = Draws(k)->pBuffer->m_pIB;

		RVertexDeclaration::Position_Normal_TexCoord_VD* pVertex = reinterpret_cast<RVertexDeclaration::Position_Normal_TexCoord_VD*>(pVB->pVertices);
		RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pSrcVertex = reinterpret_cast<RVertexDeclaration::SkeletalMesh_GPU_Skin_VD*>(Mesh.Get()->SubMeshes(0)->pVB->pVertices);

		if (Bones.Size() > 0) {
			for (unsigned int i = 0; i < pVB->nVertices; ++i) {
				TMatrix BoneTM(0);
				for (int j = 0; j < 4; ++j) {
					BoneTM += ((Bones(BoneHierarchy.Get()->Bones[pSrcVertex[i].BoneIndices[j]]->SkinBoneIndex)
						->Source->InvTM * Bones(BoneHierarchy.Get()->Bones[pSrcVertex[i].BoneIndices[j]]->SkinBoneIndex)->TM) * pSrcVertex[i].BoneWeights[j]);
				}
				pVertex[i].Position = BoneTM.TransformVector3(pSrcVertex[i].Position);
				pVertex[i].Normal = BoneTM.TransformVector3(pSrcVertex[i].Position + pSrcVertex[i].Normal);
				pVertex[i].Normal -= pVertex[i].Position;
				pVertex[i].Normal.Normalize();
			}
		}
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

void CSkeletalMeshPrimitive::CalcBoneMatrices() {
	if(AnimationTree.Get()) {
		for(unsigned int i=0;i<Bones.Size();++i) {
			Bones(i)->WeightedRot.Clear();
			Bones(i)->WeightedPos.Clear();
			Bones(i)->Weights.Clear();
		}
		AnimationTree.Get()->CalcBoneMatrices(Bones, 1.0f);
		for(unsigned int i=0;i<Bones.Size();++i) {
			TQuaternion WeightedRot = Bones(i)->WeightedRot(0);
			TVector3 WeightedPos = Bones(i)->WeightedPos(0);
			float TotalWeight = Bones(i)->Weights(0);
			for(unsigned int j=1;j<Bones(i)->Weights.Size();++j) {
				WeightedRot = TQuaternion::Slerp(Bones(i)->WeightedRot(j), WeightedRot, TotalWeight * (TotalWeight + Bones(i)->Weights(j)));
				WeightedPos = TVector3::Lerp(Bones(i)->WeightedPos(j), WeightedPos, TotalWeight * (TotalWeight + Bones(i)->Weights(j)));
				TotalWeight += Bones(i)->Weights(j);
			}
			Bones(i)->TM = TMatrix::Identity;
			Bones(i)->TM.Rotate(WeightedRot);
			Bones(i)->TM.Translate(WeightedPos);
			if(Bones(i)->Parent) {
				Bones(i)->TM *= Bones(i)->Parent->TM;
			}
		}
	}
}

RTexture* CSkeletalMeshPrimitive::GetTexture() {
	RTexture* t = new RTexture();
	t->Buffer = GDefaultTexture;
	return t;
}

CSkeletalMeshDraw::CSkeletalMeshDraw(RMesh* InMesh) {
		pBuffer = new RStaticPrimitiveBuffer();

		RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
		RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
		pBuffer->m_pVB = pVB;
		pBuffer->m_pIB = pIB;

		pVB->Protocol = RVertexProtocol::Protocols(0);//InSkeletalMesh->SkeletalSubMeshes(0)->pVB->Declaration;
		pVB->nVertices = InMesh->SubMeshes(0)->pVB->nVertices;
		pVB->pVertices = new char[pVB->Protocol->Decl->GetStride()* pVB->nVertices];
		RVertexDeclaration::Position_Normal_TexCoord_VD* pVertices = reinterpret_cast<RVertexDeclaration::Position_Normal_TexCoord_VD*>(pVB->pVertices);
		RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pSrcVertices = reinterpret_cast<RVertexDeclaration::SkeletalMesh_GPU_Skin_VD*>(InMesh->SubMeshes(0)->pVB->pVertices);
		for(unsigned int i=0;i<pVB->nVertices;++i) {
			pVertices[i].Position = pSrcVertices[i].Position;
			pVertices[i].Normal = pSrcVertices[i].Normal;
			pVertices[i].TexCoord = pSrcVertices[i].TexCoord;
		}

		pIB->nIndices = InMesh->SubMeshes(0)->pIB->nIndices;
		pIB->pIndices = new ID[pIB->nIndices];
		memcpy_s(pIB->pIndices, pIB->nIndices * sizeof(ID),
			InMesh->SubMeshes(0)->pIB->pIndices,
			pIB->nIndices * sizeof(ID));
}

CSkeletalMeshDraw::~CSkeletalMeshDraw() {
}

void TBone::CalcBoneMatrix(RAnimationBoneSequence* AnimationBoneSequence, unsigned int CurrentFrame, float Weight) {
    TQuaternion Quat = AnimationBoneSequence->GetRotKey(CurrentFrame);
	WeightedRot.AddItem(Quat);
	TVector3 Pos = AnimationBoneSequence->GetPosKey(CurrentFrame);
	WeightedPos.AddItem(Pos);
	Weights.AddItem(Weight);
}

TBone::TBone(RBone* InSource, TBone* InParent)
    :
    Source(InSource),
    Parent(InParent) {
}

TBone::~TBone() {
}