#include "StdAfx.h"
#include "CMeshPrimitive.h"
#include "BDriver.h"
#include "RMesh.h"

CMeshDraw::CMeshDraw(RMesh* InMesh) :
	Mesh(InMesh) {
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
		pIB->pIndices = new TIndex16[pIB->nIndices];
		memcpy_s(pIB->pIndices, pIB->nIndices * sizeof(TIndex16),
			InMesh->SkeletalSubMeshes(0)->pIB->pIndices,
			pIB->nIndices * sizeof(TIndex16));

		UpdatePrimitive();
}

CMeshPrimitive::CMeshPrimitive(void) {
	RenderType = RenderType_Opaque;
}

CMeshPrimitive::~CMeshPrimitive(void) {
}

void CMeshPrimitive::SetMesh(RMesh* InMesh) {
	Mesh.Set(InMesh);
}

bool CMeshPrimitive::Access(AAccessor& Accessor) {
	__super::Access(Accessor);
	Accessor << Mesh;
	if(Accessor.IsLoading()) {
		CreateDraws();
	}
	return true;
}

void CMeshPrimitive::CreateDraws() {
	Draws.AddItem(new CMeshDraw(Mesh.Get()));
}

void CMeshPrimitive::UpdatePrimitive() {
	NumIndices = 0;
	for (unsigned int i = 0; i < Draws.Size(); ++i) {
		Draws(i)->UpdatePrimitive();
		NumIndices += Draws(i)->pBuffer->m_pIB->nIndices;
	}
}

RMaterial* CMeshPrimitive::GetMaterial() {
	return RMaterialTable::Materials(0);
}

unsigned int CMeshPrimitive::FillDynamicVertexBuffer(char** pData) {
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

unsigned int CMeshPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) {
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

unsigned int CMeshPrimitive::GetNumIndices() {
	return NumIndices;
}