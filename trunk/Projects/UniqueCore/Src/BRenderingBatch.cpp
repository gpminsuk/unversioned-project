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
	Primitives.Clear(true);
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
		case RenderType_Line:
		case RenderType_Opaque:
			GOpaqueBasePass->DrawPrimitive(this);
			break;
		/*case RenderType_UI:
			GDrawFontPass->BeginPass(Viewport);
			GDrawFontPass->DrawPrimitive(this);
			GDrawFontPass->EndPass();
			break;*/
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
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		delete Batch;
	}
}

void BRenderingBatchManager::RenderBatches(BViewport* Viewport)
{
	GOpaqueBasePass->BeginPass(Viewport);
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		Batch->RenderBatch(Viewport);
	}
	GOpaqueBasePass->EndPass();
}

void BRenderingBatchManager::Syncronize()
{
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		Batch->Syncronize();
	}
}

void BRenderingBatchManager::RemovePrimitive(BPrimitive* Primitive)
{
	for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		if(Batch->nVertexStride == Primitive->GetVertexStride())
		{
			Primitive->RemoveRender(Batch);
			Batch->Primitives.DeleteItemByVal(Primitive);
			return;
		}
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
			Batch->Primitives.AddItem(Primitive);
			return;
		}
	}
	BRenderingBatch* Batch = new BRenderingBatch();
	Batch->RenderType =  Primitive->RenderType;
	if(Primitive->RenderType == RenderType_Line)
	{
		Batch->PrimitiveType =  PrimitiveType_LineList;
	}
	else
	{
		Batch->PrimitiveType =  PrimitiveType_TriangleList;
	}
	RenderingBatches.AddItem(Batch);
	Primitive->Render(Batch);
	Batch->Primitives.AddItem(Primitive);
}