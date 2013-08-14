#include "StdAfx.h"
#include "CMeshPrimitive.h"
#include "BDriver.h"

CMeshDraw::CMeshDraw(RMesh* InMesh) :
	Mesh(InMesh) {

}

IMPLEMENT_CLASS(CMeshPrimitive)

CMeshPrimitive::CMeshPrimitive(void) {
}

CMeshPrimitive::~CMeshPrimitive(void) {
}

void CMeshPrimitive::SetMesh(RMesh* InMesh) {
	Mesh.Set(InMesh);
}

bool CMeshPrimitive::Access(AAccessor& Accessor) {
	__super::Access(Accessor);
	Accessor << Mesh;
	Accessor << Texture;
	if(Accessor.IsLoading()) {
		CreateDraws();
	}
	return true;
}

void CMeshPrimitive::CreateDraws() {
	Draws.AddItem(new CMeshDraw(Mesh.Get()));
}

RMaterial* CMeshPrimitive::GetMaterial() {
	return RMaterialTable::Materials(0);
}

unsigned int CMeshPrimitive::FillDynamicVertexBuffer(char** pData) {
	GDriver->SetTexture(0, Texture.Get());

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