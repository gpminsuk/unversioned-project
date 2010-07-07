#include "stdafx.h"
#include "CUIButton.h"
#include "BRenderingBatch.h"

TUIButtonPrimitive::TUIButtonPrimitive()
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

	pVB->nVertices = 6;
	pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

	VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);

	Vertex[0].Pos =  TVector3(-1.0f,-1.0f,0.0f);
	Vertex[0].UV  =  TVector2(0.0f,1.0f);
	Vertex[1].Pos =  TVector3( 1.0f,-1.0f,0.0f);
	Vertex[1].UV  =  TVector2(1.0f,1.0f);
	Vertex[2].Pos =  TVector3( 1.0f, 1.0f,0.0f);
	Vertex[2].UV  =  TVector2(1.0f,0.0f);
	Vertex[3].Pos =  TVector3(-1.0f,-1.0f,0.0f);
	Vertex[3].UV  =  TVector2(0.0f,1.0f);
	Vertex[4].Pos =  TVector3( 1.0f, 1.0f,0.0f);
	Vertex[4].UV  =  TVector2(1.0f,0.0f);
	Vertex[5].Pos =  TVector3(-1.0f, 1.0f,0.0f);
	Vertex[5].UV  =  TVector2(0.0f,0.0f);

	pIB->nIndices = 2;
	pIB->pIndices = new TIndex16[pIB->nIndices];

	TIndex16 *Index = reinterpret_cast<TIndex16*>(pIB->pIndices);

	Index[0] = TIndex16(0,2,1);
	Index[1] = TIndex16(3,5,4);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CUIButtonPrimitive::CUIButtonPrimitive()
{
	RenderType = RenderType_UI;

	TUIButtonPrimitive* ButtonPrimitive = new TUIButtonPrimitive();
	Primitives.AddItem(ButtonPrimitive);
}

CUIButtonPrimitive::~CUIButtonPrimitive(void)
{
}

void CUIButtonPrimitive::Render(BRenderingBatch *Batch)
{
	Batch->nVertexStride = Primitives(0)->pBuffer->m_pVB->nVertexStride;
	Batch->nVertices += Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CUIButtonPrimitive::GetNumIndices()
{
	return Primitives(0)->pBuffer->m_pIB->nIndices;
}

unsigned int CUIButtonPrimitive::FillDynamicVertexBuffer(char** pData)
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

unsigned int CUIButtonPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex)
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

BSynchronizer* CUIButtonPrimitive::CreateSynchronizer()
{
	Syncronizer = new CUIButtonSyncronizer();
	return Syncronizer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CUIButtonComponent::CUIButtonComponent()
:	Width(100),
	Height(100)
{
	CUIButtonPrimitive* ButtonPrimitive = new CUIButtonPrimitive();
	Primitives.AddItem(ButtonPrimitive);

	SyncronizerRef = ButtonPrimitive->CreateSynchronizer();
	SendSyncData();
}

CUIButtonComponent::~CUIButtonComponent()
{

}

void CUIButtonComponent::UpdateComponent()
{	
	SendSyncData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CUIButtonSyncronizer::CUIButtonSyncronizer()
{
	SyncData = new CUIButtonSyncronizeData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CUIButtonSyncronizeData::SetData(BComponent* Component)
{
	CUIButtonComponent* ButtonComponent = reinterpret_cast<CUIButtonComponent*>(Component);
	if(ButtonComponent)
	{
		Width = ButtonComponent->Width;
		Height = ButtonComponent->Height;
	}
}

void CUIButtonSyncronizeData::GetData(BPrimitive* Primitive)
{
	CUIButtonPrimitive* ButtonPrimitive = reinterpret_cast<CUIButtonPrimitive*>(Primitive);
	if(ButtonPrimitive)
	{
		Width = ButtonPrimitive->Width;
		Height = ButtonPrimitive->Height;
	}
}