#include "StdAfx.h"
#include "BRenderingBatch.h"
#include "BRenderer.h"
#include "BPrimitive.h"
#include "BDriver.h"

#include "BOpaqueBasePass.h"
#include "BDrawUIPass.h"

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
		switch(RenderType)
		{
		case RenderType_Opaque:
			GOpaqueBasePass->BeginPass(Viewport);
			GOpaqueBasePass->DrawPrimitive(this);
			GOpaqueBasePass->EndPass();
			break;
		case RenderType_UI:
			GDrawFontPass->BeginPass(Viewport);
			GDrawFontPass->DrawPrimitive(this);
			GDrawFontPass->EndPass();
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
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		Batch->Syncronize();
	}
}

void BRenderingBatchManager::AddPrimitive(BPrimitive* Primitive)
{
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		if(Batch->nVertexStride == Primitive->GetVertexStride())
		{
			Primitive->Render(Batch);
			return;
		}
	}
	BRenderingBatch* Batch = new BRenderingBatch();
	Batch->RenderType =  Primitive->RenderType;
	Batch->PrimitiveType =  PrimitiveType_TriangleList;
	RenderingBatches.AddItem(Batch);
	Primitive->Render(Batch);
	Batch->Primitives.AddItem(Primitive);
}