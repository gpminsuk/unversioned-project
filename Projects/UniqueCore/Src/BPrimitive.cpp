#include "StdAfx.h"
#include "BPrimitive.h"
#include "BRenderingBatch.h"

#include "RResource.h"

BPrimitive::BPrimitive()
{
}

BPrimitive::~BPrimitive(void)
{
	Primitives.Clear();
}

void BPrimitive::Render(BRenderingBatch *Batch)
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		TPrimitive* Prim = Primitives(i);
		if(Prim)
		{
			Batch->nVertexStride = Prim->pBuffer->m_pVB->nVertexStride;
			Batch->nVertices += Prim->pBuffer->m_pVB->nVertices;
		}		
	}
}

void BPrimitive::RemoveRender(BRenderingBatch *Batch)
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		TPrimitive* Prim = Primitives(i);
		if(Prim)
		{
			Batch->nVertexStride = Prim->pBuffer->m_pVB->nVertexStride;
			Batch->nVertices -= Prim->pBuffer->m_pVB->nVertices;
		}		
	}
}

TPrimitive::TPrimitive()
: pBuffer(0)
{
}

TPrimitive::~TPrimitive()
{
	delete pBuffer;
}