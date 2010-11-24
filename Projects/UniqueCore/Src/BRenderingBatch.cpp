#include "StdAfx.h"
#include "BRenderingBatch.h"
#include "BRenderer.h"
#include "BPrimitive.h"
#include "BDriver.h"

#include "BOpaqueBasePass.h"

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

void BRenderingBatch::RenderBatch(BViewport* Viewport)
{
	PrimitiveBuffer = GDriver->CreatePrimitiveBuffer(this);	
	if(PrimitiveBuffer)
	{
		switch(RenderingPassType)
		{
		case RenderingPass_Opaque:
			GOpaqueBasePass->BeginPass(Viewport);
			GOpaqueBasePass->DrawPrimitive(this);
			GOpaqueBasePass->EndPass();
			break;
		}
		PrimitiveBuffer->Release();
		delete PrimitiveBuffer;
	}
}

void BRenderingBatch::Syncronize()
{
}

BRenderingBatchManager::BRenderingBatchManager()
{

}

BRenderingBatchManager::~BRenderingBatchManager()
{

}

void BRenderingBatchManager::RenderBatches(BViewport* Viewport)
{
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		Batch->RenderBatch(Viewport);
	}
}

void BRenderingBatchManager::Syncronize()
{
	static BRenderingBatch* Batch = 0;
	if(!Batch)
	{
		Batch = new BRenderingBatch();
		Batch->RenderingPassType =  RenderingPass_Opaque;
		Batch->RenderType =  PrimitiveType_TriangleList;
		for(unsigned int i=0;i<RenderPrimitives.Size();++i)
		{
			RenderPrimitives(i)->Render(Batch);			
		}
		RenderingBatches.AddItem(Batch);
	}
	Batch->Primitives.Clear();
	Batch->Primitives = RenderPrimitives;
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		Batch->Syncronize();
	}
}

void BRenderingBatchManager::AddPrimitive(BPrimitive* Primitive)
{
	RenderPrimitives.AddItem(Primitive);
	if(RenderingBatches.Size())
		Primitive->Render(RenderingBatches(0));
}