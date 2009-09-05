#include "StdAfx.h"
#include "CBoxPrimitive.h"

#include "RResource.h"

TBoxPrimitive* BoxPrimitive;

CBoxPrimitive::CBoxPrimitive(void)
{
	RenderType = RenderType_Line;
	Primitives.AddItem(BoxPrimitive);
}

CBoxPrimitive::~CBoxPrimitive(void)
{
}

TBoxPrimitive::TBoxPrimitive()
{
	pBuffer = new RStaticPrimitiveBuffer();

	RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
	RSystemMemoryVertexBufferTable::VertexBuffers.AddItem(pVB);
	RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
	RSystemMemoryIndexBufferTable::IndexBuffers.AddItem(pIB);
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

	Vertex[0].Pos =  TVector3(-1.0f,-1.0f,-1.0f);
	Vertex[1].Pos =  TVector3( 1.0f,-1.0f,-1.0f);
	Vertex[2].Pos =  TVector3( 1.0f,-1.0f,-1.0f);
	Vertex[3].Pos =  TVector3( 1.0f, 1.0f,-1.0f);
	Vertex[4].Pos =  TVector3( 1.0f, 1.0f,-1.0f);
	Vertex[5].Pos =  TVector3(-1.0f, 1.0f,-1.0f);
	Vertex[6].Pos =  TVector3(-1.0f, 1.0f,-1.0f);
	Vertex[7].Pos =  TVector3(-1.0f,-1.0f,-1.0f);

	Vertex[8].Pos =  TVector3(-1.0f,-1.0f, 1.0f);
	Vertex[9].Pos =  TVector3( 1.0f,-1.0f, 1.0f);
	Vertex[10].Pos = TVector3( 1.0f,-1.0f, 1.0f);
	Vertex[11].Pos = TVector3( 1.0f, 1.0f, 1.0f);
	Vertex[12].Pos = TVector3( 1.0f, 1.0f, 1.0f);
	Vertex[13].Pos = TVector3(-1.0f, 1.0f, 1.0f);
	Vertex[14].Pos = TVector3(-1.0f, 1.0f, 1.0f);
	Vertex[15].Pos = TVector3(-1.0f,-1.0f, 1.0f);

	Vertex[16].Pos = TVector3(-1.0f,-1.0f,-1.0f);
	Vertex[17].Pos = TVector3(-1.0f,-1.0f, 1.0f);
	Vertex[18].Pos = TVector3(-1.0f, 1.0f,-1.0f);
	Vertex[19].Pos = TVector3(-1.0f, 1.0f, 1.0f);
	Vertex[20].Pos = TVector3( 1.0f,-1.0f,-1.0f);
	Vertex[21].Pos = TVector3( 1.0f,-1.0f, 1.0f);
	Vertex[22].Pos = TVector3( 1.0f, 1.0f,-1.0f);
	Vertex[23].Pos = TVector3( 1.0f, 1.0f, 1.0f);

	pIB->nIndices = 0;
	/*
	pVB->nVertices = 24;
	pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

	VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);

	Vertex[0].Pos = TVector3(-1.0f,-1.0f,-1.0f);
	Vertex[1].Pos = TVector3( 1.0f,-1.0f,-1.0f);
	Vertex[2].Pos = TVector3(-1.0f, 1.0f,-1.0f);
	Vertex[3].Pos = TVector3( 1.0f, 1.0f,-1.0f);

	Vertex[4].Pos = TVector3( 1.0f,-1.0f,-1.0f);
	Vertex[5].Pos = TVector3( 1.0f,-1.0f, 1.0f);
	Vertex[6].Pos = TVector3( 1.0f, 1.0f,-1.0f);
	Vertex[7].Pos = TVector3( 1.0f, 1.0f, 1.0f);

	Vertex[8].Pos = TVector3( 1.0f,-1.0f, 1.0f);
	Vertex[9].Pos = TVector3(-1.0f,-1.0f, 1.0f);
	Vertex[10].Pos = TVector3( 1.0f, 1.0f, 1.0f);
	Vertex[11].Pos = TVector3(-1.0f, 1.0f, 1.0f);

	Vertex[12].Pos = TVector3(-1.0f,-1.0f, 1.0f);
	Vertex[13].Pos = TVector3(-1.0f,-1.0f,-1.0f);
	Vertex[14].Pos = TVector3(-1.0f, 1.0f, 1.0f);
	Vertex[15].Pos = TVector3(-1.0f, 1.0f,-1.0f);

	Vertex[16].Pos = TVector3(-1.0f, 1.0f,-1.0f);
	Vertex[17].Pos = TVector3( 1.0f, 1.0f,-1.0f);
	Vertex[18].Pos = TVector3(-1.0f, 1.0f, 1.0f);
	Vertex[19].Pos = TVector3( 1.0f, 1.0f, 1.0f);

	Vertex[20].Pos = TVector3( 1.0f,-1.0f, 1.0f);
	Vertex[21].Pos = TVector3(-1.0f,-1.0f, 1.0f);
	Vertex[22].Pos = TVector3( 1.0f,-1.0f,-1.0f);
	Vertex[23].Pos = TVector3(-1.0f,-1.0f,-1.0f);

	pIB->nIndices = 12;
	pIB->pIndices = new TIndex16[pIB->nIndices];

	TIndex16 *Index = pIB->pIndices;

	Index[0] = TIndex16(0,2,3);		Index[1] = TIndex16(0,3,1);
	Index[2] = TIndex16(4,6,7);		Index[3] = TIndex16(4,7,5);
	Index[4] = TIndex16(8,10,11);	Index[5] = TIndex16(8,11,9);
	Index[6] = TIndex16(12,14,15);	Index[7] = TIndex16(12,15,13);
	Index[8] = TIndex16(16,18,19);	Index[9] = TIndex16(16,19,17);
	Index[10] = TIndex16(20,22,23);	Index[11] = TIndex16(20,23,21);*/
}

void CBoxPrimitive::Render(TBatch *Batch)
{
	Batch->nVertexStride = Primitives(0)->pBuffer->m_pVB->nVertexStride;
	Batch->nVertices += Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CBoxPrimitive::GetNumIndices()
{
	return Primitives(0)->pBuffer->m_pIB->nIndices;
}

unsigned int CBoxPrimitive::FillDynamicVertexBuffer(char** pData)
{
	memcpy((*pData), Primitives(0)->pBuffer->m_pVB->pVertices, 
		Primitives(0)->pBuffer->m_pVB->nVertices * Primitives(0)->pBuffer->m_pVB->nVertexStride);
	for(int k=0;k<Primitives(0)->pBuffer->m_pVB->nVertices;++k)
	{
		*((TVector3*)&((*pData)[k*Primitives(0)->pBuffer->m_pVB->nVertexStride])) = TM.TransformVector3(*((TVector3*)&((*pData)[k*Primitives(0)->pBuffer->m_pVB->nVertexStride])));
	}
	*pData += Primitives(0)->pBuffer->m_pVB->nVertices * Primitives(0)->pBuffer->m_pVB->nVertexStride;
	return Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CBoxPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex)
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
