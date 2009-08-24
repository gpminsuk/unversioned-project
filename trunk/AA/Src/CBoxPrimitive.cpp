#include "StdAfx.h"
#include "CBoxPrimitive.h"

#include "RResource.h"

TBoxPrimitive* BoxPrimitive;

CBoxPrimitive::CBoxPrimitive(void)
{
	Primitives.AddItem(BoxPrimitive);
}

CBoxPrimitive::~CBoxPrimitive(void)
{
}

TBoxPrimitive::TBoxPrimitive()
{
	pBuffer = new RStaticPrimitiveBuffer();

	RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
	RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
	pBuffer->m_pVB = pVB;
	pBuffer->m_pIB = pIB;

	pVB->Declaration = new VertexDeclaration[2];
	pVB->Declaration[0].Offset = 0;
	pVB->Declaration[0].Type = DECLTYPE_FLOAT3;	// Position
	pVB->Declaration[1].Offset = 12;
	pVB->Declaration[1].Type = DECLTYPE_FLOAT2;	// UV

	struct VD
	{
		TVector3 Pos;
		TVector2 UV;
	};

	pVB->nVertexStride = sizeof(VD);
	pVB->nVertices = 24;
	pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

	VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);

	Vertex[0].Pos = TVector3(-1.0f,-1.0f,-1.0f);	Vertex[0].UV = TVector2(0.0f,0.0f);
	Vertex[1].Pos = TVector3( 1.0f,-1.0f,-1.0f);	Vertex[1].UV = TVector2(0.0f,0.0f);
	Vertex[2].Pos = TVector3(-1.0f, 1.0f,-1.0f);	Vertex[2].UV = TVector2(0.0f,0.0f);
	Vertex[3].Pos = TVector3( 1.0f, 1.0f,-1.0f);	Vertex[3].UV = TVector2(0.0f,0.0f);

	Vertex[4].Pos = TVector3( 1.0f,-1.0f,-1.0f);	Vertex[4].UV = TVector2(0.0f,0.0f);
	Vertex[5].Pos = TVector3( 1.0f,-1.0f, 1.0f);	Vertex[5].UV = TVector2(0.0f,0.0f);
	Vertex[6].Pos = TVector3( 1.0f, 1.0f,-1.0f);	Vertex[6].UV = TVector2(0.0f,0.0f);
	Vertex[7].Pos = TVector3( 1.0f, 1.0f, 1.0f);	Vertex[7].UV = TVector2(0.0f,0.0f);

	Vertex[8].Pos = TVector3( 1.0f,-1.0f, 1.0f);	Vertex[8].UV = TVector2(0.0f,0.0f);
	Vertex[9].Pos = TVector3(-1.0f,-1.0f, 1.0f);	Vertex[9].UV = TVector2(0.0f,0.0f);
	Vertex[10].Pos = TVector3( 1.0f, 1.0f, 1.0f);	Vertex[10].UV = TVector2(0.0f,0.0f);
	Vertex[11].Pos = TVector3(-1.0f, 1.0f, 1.0f);	Vertex[11].UV = TVector2(0.0f,0.0f);

	Vertex[12].Pos = TVector3(-1.0f,-1.0f, 1.0f);	Vertex[12].UV = TVector2(0.0f,0.0f);
	Vertex[13].Pos = TVector3(-1.0f,-1.0f,-1.0f);	Vertex[13].UV = TVector2(0.0f,0.0f);
	Vertex[14].Pos = TVector3(-1.0f, 1.0f, 1.0f);	Vertex[14].UV = TVector2(0.0f,0.0f);
	Vertex[15].Pos = TVector3(-1.0f, 1.0f,-1.0f);	Vertex[15].UV = TVector2(0.0f,0.0f);

	Vertex[16].Pos = TVector3(-1.0f, 1.0f,-1.0f);	Vertex[16].UV = TVector2(0.0f,0.0f);
	Vertex[17].Pos = TVector3( 1.0f, 1.0f,-1.0f);	Vertex[17].UV = TVector2(0.0f,0.0f);
	Vertex[18].Pos = TVector3(-1.0f, 1.0f, 1.0f);	Vertex[18].UV = TVector2(0.0f,0.0f);
	Vertex[19].Pos = TVector3( 1.0f, 1.0f, 1.0f);	Vertex[19].UV = TVector2(0.0f,0.0f);

	Vertex[20].Pos = TVector3( 1.0f,-1.0f, 1.0f);	Vertex[20].UV = TVector2(0.0f,0.0f);
	Vertex[21].Pos = TVector3(-1.0f,-1.0f, 1.0f);	Vertex[21].UV = TVector2(0.0f,0.0f);
	Vertex[22].Pos = TVector3( 1.0f,-1.0f,-1.0f);	Vertex[22].UV = TVector2(0.0f,0.0f);
	Vertex[23].Pos = TVector3(-1.0f,-1.0f,-1.0f);	Vertex[23].UV = TVector2(0.0f,0.0f);

	pIB->nIndices = 12;
	pIB->pIndices = new TIndex16[pIB->nIndices];

	TIndex16 *Index = pIB->pIndices;

	Index[0] = TIndex16(0,2,3);		Index[1] = TIndex16(0,3,1);
	Index[2] = TIndex16(4,6,7);		Index[3] = TIndex16(4,7,5);
	Index[4] = TIndex16(8,10,11);	Index[5] = TIndex16(8,11,9);
	Index[6] = TIndex16(12,14,15);	Index[7] = TIndex16(12,15,13);
	Index[8] = TIndex16(16,18,19);	Index[9] = TIndex16(16,19,17);
	Index[10] = TIndex16(20,22,23);	Index[11] = TIndex16(20,23,21);
}

void TBoxPrimitive::Render(TBatch *Batch)
{
	Batch->nVertexStride = pBuffer->m_pVB->nVertexStride;
	Batch->nVertices += pBuffer->m_pVB->nVertices;
}

unsigned int TBoxPrimitive::GetNumIndices()
{
	return pBuffer->m_pIB->nIndices;
}

unsigned int TBoxPrimitive::FillDynamicVertexBuffer(char** pData)
{
	memcpy((*pData), pBuffer->m_pVB->pVertices, 
		pBuffer->m_pVB->nVertices * pBuffer->m_pVB->nVertexStride);
	for(int k=0;k<pBuffer->m_pVB->nVertices;++k)
	{
		*((TVector3*)&((*pData)[k*pBuffer->m_pVB->nVertexStride])) = TM.TransformVector3(*((TVector3*)&((*pData)[k*pBuffer->m_pVB->nVertexStride])));
	}
	*pData += pBuffer->m_pVB->nVertices * pBuffer->m_pVB->nVertexStride;
	return pBuffer->m_pVB->nVertices;
}

unsigned int TBoxPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex)
{
	for(unsigned int k=0;k<GetNumIndices();++k)
	{
		TIndex16 tmpIndex;
		tmpIndex._1 = pBuffer->m_pIB->pIndices[k]._1 + *BaseIndex;
		tmpIndex._2 = pBuffer->m_pIB->pIndices[k]._2 + *BaseIndex;
		tmpIndex._3 = pBuffer->m_pIB->pIndices[k]._3 + *BaseIndex;
		(*pData)[k] = tmpIndex;
	}
	*BaseIndex += pBuffer->m_pVB->nVertices;
	*pData += GetNumIndices();

	return pBuffer->m_pVB->nVertices;
}
