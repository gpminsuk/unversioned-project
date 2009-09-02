#include "StdAfx.h"
#include "CCylinderPrimitive.h"

#include "RResource.h"

TCylinderPrimitive* CylinderPrimitive;

CCylinderPrimitive::CCylinderPrimitive(void)
{
	Primitives.AddItem(CylinderPrimitive);
}

CCylinderPrimitive::~CCylinderPrimitive(void)
{
}

TCylinderPrimitive::TCylinderPrimitive()
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

	int Smoothness = 24;

	pVB->nVertexStride = sizeof(VD);
	pVB->nVertices = Smoothness*6;
	pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

	VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);

	float Theta = 0.0f;
	for(int i=0;i<pVB->nVertices/6;++i)
	{
		Vertex[i*6+0].Pos = TVector3(COSINE(Theta), 1.0f, SINE(Theta));
		Vertex[i*6+1].Pos = TVector3(COSINE(Theta), 0.0f, SINE(Theta));
		Vertex[i*6+2].Pos = TVector3(COSINE(Theta), 1.0f, SINE(Theta));
		Vertex[i*6+4].Pos = TVector3(COSINE(Theta), 0.0f, SINE(Theta));
		Theta += (MATH_PI*2.0f)/((float)pVB->nVertices/6.0f);
		Vertex[i*6+3].Pos = TVector3(COSINE(Theta), 1.0f, SINE(Theta));
		Vertex[i*6+5].Pos = TVector3(COSINE(Theta), 0.0f, SINE(Theta));
	}

	pIB->nIndices = 0;
	/*
	pIB->pIndices = new TIndex16[pIB->nIndices];

	TIndex16 *Index = pIB->pIndices;
	for(int i=0;i<pIB->nIndices/2;++i)
	{
		if(i == (pIB->nIndices/2)-1)
		{
			//Index[i*2+0] = TIndex16(i*2,0,0);
			//Index[i*2+1] = TIndex16((i*2)+1,1,0);
		}
		else
		{
			//Index[i*2+0] = TIndex16(i*2,(i*2)+2,0);
			//Index[i*2+1] = TIndex16((i*2)+1,(i*2)+3,0);
		}
		//Index[i*2+0] = TIndex16(0,1,2);
		//Index[i*2+1] = TIndex16(1,3,2);
	}*/
}

void CCylinderPrimitive::Render(TBatch *Batch)
{
	Batch->nVertexStride = Primitives(0)->pBuffer->m_pVB->nVertexStride;
	Batch->nVertices += Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CCylinderPrimitive::GetNumIndices()
{
	return Primitives(0)->pBuffer->m_pIB->nIndices;
}

unsigned int CCylinderPrimitive::FillDynamicVertexBuffer(char** pData)
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

unsigned int CCylinderPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex)
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
