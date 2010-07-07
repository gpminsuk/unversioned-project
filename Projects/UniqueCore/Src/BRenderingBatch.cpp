#include "StdAfx.h"
#include "BRenderingBatch.h"
#include "BPrimitive.h"

BRenderingBatch::BRenderingBatch()
:	nVertices(0),
	nVertexStride(0)
{
}

BRenderingBatch::~BRenderingBatch()
{
}

int BRenderingBatch::GetNumIndices()
{
	int NumIndices = 0;
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		NumIndices += Primitives(i)->GetNumIndices();
	}
	return NumIndices;
}

void BRenderingBatch::IndexTessellate()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		Primitives(i)->IndexTessellate();
	}
}

void BRenderingBatch::Syncronize()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		BPrimitive* Primitive = Primitives(i);
		Primitive->Syncronize();
	}
}

void BRenderingBatchManager::RenderBatch()
{
	
}

void BRenderingBatchManager::Syncronize()
{
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		Batch->Syncronize();
	}
}