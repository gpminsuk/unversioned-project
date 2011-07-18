#include "StdAfx.h"
#include "BRenderingBatch.h"
#include "BRenderer.h"
#include "BPrimitive.h"
#include "BDriver.h"

#include "BOpaqueBasePass.h"
#include "BDirectionalLightPass.h"
#include "BDrawUIPass.h"
#include "BViewport.h"
#include "BLightComponent.h"

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

void BRenderingBatch::ConfigureShader()
{
	Shader->PixelShader->Configure->ConfigureLight(this);
	Shader->VertexShader->Configure->ConfigureLight(this);
}

void BRenderingBatch::RenderLight()
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
		case RenderType_UI:
			break;
		}
		PrimitiveBuffer->Release();
		delete PrimitiveBuffer;
	}
}

void BRenderingBatch::RenderBaseScene()
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
		case RenderType_UI:
			GDrawFontPass->DrawPrimitive(this);
			break;
		}
		PrimitiveBuffer->Release();
		delete PrimitiveBuffer;
	}
}

BRenderingBatchManager::BRenderingBatchManager()
{
}

BRenderingBatchManager::~BRenderingBatchManager()
{
}

void BRenderingBatchManager::RenderBatches(BViewport* Viewport)
{
	GOpaqueBasePass->BeginPass(Viewport);
	for(unsigned int i=0;i<Batches.Size();++i)
	{
		BRenderingBatch* Batch = Batches(i);

		GOpaqueBasePass->BeginRenderBatch(Batch);
		{
			Batch->RenderBaseScene();
		}
		GOpaqueBasePass->EndRenderBatch();
	}
	GOpaqueBasePass->EndPass();

	GDirectionalLightPass->BeginPass(Viewport);
	for(unsigned int i=0;i<Viewport->Lights.Size();++i)
	{
		for(unsigned int i=0;i<Batches.Size();++i)
		{
			BRenderingBatch* Batch = Batches(i);
			Batch->Lights = Viewport->Lights;
			GDirectionalLightPass->BeginRenderBatch(Batch);
			{
				Batch->ConfigureShader();
				Batch->RenderLight();
			}
			GDirectionalLightPass->EndRenderBatch();
		}
	}
	GDirectionalLightPass->EndPass();	
}
void BRenderingBatchManager::RemovePrimitive(BPrimitive* Primitive)
{
	for(unsigned int i=0;i<Batches.Size();++i)
	{
		BRenderingBatch* Batch = Batches(i);
		if(Batch->Primitives(0) == Primitive)
		{
			Primitive->RemoveRender(Batch);
			Batch->Primitives.DeleteItemByVal(Primitive);
			Batches.DeleteItem(i);
			delete Batch;
			return;
		}
	}
}

void BRenderingBatchManager::AddPrimitive(BPrimitive* Primitive)
{	
	// TODO
	/*for(unsigned int i=0;i<RenderingBatches.Size();++i)
	{
		BRenderingBatch* Batch = RenderingBatches(i);
		if(Batch->nVertexStride == Primitive->GetVertexStride())
		{
			Primitive->Render(Batch);
			Batch->Primitives.AddItem(Primitive);
			return;
		}
	}*/
	BRenderingBatch* Batch = new BRenderingBatch();
	Batch->Shader = RShaderTable::Shaders(0);
	Batch->RenderType =  Primitive->RenderType;
	if(Primitive->RenderType == RenderType_Line)
	{
		Batch->PrimitiveType = PrimitiveType_LineList;
	}
	else
	{
		Batch->PrimitiveType = PrimitiveType_TriangleList;
	}
	Batches.AddItem(Batch);
	Primitive->Render(Batch);
	Batch->Primitives.AddItem(Primitive);
}