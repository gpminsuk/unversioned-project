#include "StdAfx.h"
#include "BRenderingBatch.h"
#include "BRenderer.h"
#include "BPrimitive.h"
#include "BDriver.h"

#include "RTexture.h"
#include "BLineBatcher.h"
#include "BTextDrawer.h"
#include "BRTRenderPass.h"
#include "BOpaqueBasePass.h"
#include "BDirectionalLightPass.h"
#include "BDrawUIPass.h"
#include "BViewport.h"

BRenderingBatch::BRenderingBatch(BPrimitive* InitialPrimitive)
    :
    nVertices(0),
	isDirty(true),
	PrimitiveBuffer(0) {
	BatchPrimitive(InitialPrimitive);
}

BRenderingBatch::~BRenderingBatch() {
    Primitives.Clear(true);
}

bool BRenderingBatch::IsBatchable(BPrimitive* Primitive) {	
	return (pMaterial == Primitive->GetMaterial() && RenderType == Primitive->RenderType && Protocol == Primitive->Draws(0)->pBuffer->m_pVB->Protocol) &&
		(nVertices < (1 << (sizeof(short) * 8))) && false;
}

void BRenderingBatch::RemovePrimitive(BPrimitive* Primitive) {
	for (unsigned int i = 0; i < Primitive->Draws.Size(); ++i) {
		BDraw* Prim = Primitive->Draws(i);
		if (Prim) {
			nVertices -= Prim->pBuffer->m_pVB->nVertices;
		}
	}
	Primitives.DeleteItemByVal(Primitive);
}

void BRenderingBatch::BatchPrimitive(BPrimitive* Primitive) {
	pMaterial = Primitive->GetMaterial();
	RenderType = Primitive->RenderType;
	PrimitiveType = PrimitiveType_TriangleList;
	Texture = Primitive->GetTexture()->Buffer;
	for (unsigned int i = 0; i < Primitive->Draws.Size(); ++i) {
		BDraw* Prim = Primitive->Draws(i);
		if (Prim) {
			Protocol = Prim->pBuffer->m_pVB->Protocol;
			nVertices += Prim->pBuffer->m_pVB->nVertices;
		}
	}
	Primitives.AddItem(Primitive);
}

int BRenderingBatch::GetNumIndices() {
    int NumIndices = 0;
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        NumIndices += Primitives(i)->GetNumIndices();
    }
    return NumIndices;
}

void BRenderingBatch::IndexTessellate() {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        Primitives(i)->IndexTessellate();
    }
}

void BRenderingBatch::ConfigureShader() {
	RShaderPass::ShaderPasses(0)->ConfigureLight(pMaterial->Shaders(0)->PixelShader, pMaterial->Shaders(0)->VertexShader, this);
}

void BRenderingBatch::RenderLight() {
	GDriver->SetStreamSource(PrimitiveBuffer);
	GDriver->SetIndices(PrimitiveBuffer);
    if (PrimitiveBuffer) {
        switch (RenderType) {
        case RenderType_Line:
        case RenderType_Opaque:
            GOpaqueBasePass->DrawPrimitive(this);
            break;
        case RenderType_UI:
            break;
        }
    }
}

void BRenderingBatch::RenderBaseScene() {
	if(isDirty) {
		if(PrimitiveBuffer) {
			PrimitiveBuffer->Release();
			delete PrimitiveBuffer;
		}
		PrimitiveBuffer = GDriver->CreatePrimitiveBuffer(this);
		isDirty = false;
	}
	GDriver->SetStreamSource(PrimitiveBuffer);
	GDriver->SetIndices(PrimitiveBuffer);
	GDriver->SetTexture(0, Texture);
    if (PrimitiveBuffer) {
        switch (RenderType) {
        case RenderType_Line:
        case RenderType_Opaque:
            GOpaqueBasePass->DrawPrimitive(this);
            break;
        case RenderType_UI:
            GDrawFontPass->DrawPrimitive(this);
            break;
        }
    }
}

BRenderingBatchManager::BRenderingBatchManager() {
	LineBatcher = new BLineBatcher();
}

BRenderingBatchManager::~BRenderingBatchManager() {
}

void BRenderingBatchManager::RenderBatches() {
	for (UINT i = 0; i < m_Viewports->Size(); ++i) {
		BViewport* Viewport = (*m_Viewports)(i);
		GDriver->BeginScene(Viewport);

		GOpaqueBasePass->BeginPass(Viewport);
		for (unsigned int i = 0; i < Batches.Size(); ++i) {
			BRenderingBatch* Batch = Batches(i);

			GOpaqueBasePass->BeginRenderBatch(Batch);
			{
				Batch->RenderBaseScene();
			}
			GOpaqueBasePass->EndRenderBatch();
		}
		GOpaqueBasePass->EndPass();

		if(Lights.Size() > 0) {
			GDirectionalLightPass->BeginPass(Viewport);
			for (unsigned int i = 0; i < Lights.Size(); ++i) {
				for (unsigned int i = 0; i < Batches.Size(); ++i) {
					BRenderingBatch* Batch = Batches(i);
					Batch->Lights = Lights;
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

		GBaseRTRenderPass->BeginPass(Viewport);
		GBaseRTRenderPass->DrawPrimitive();
		GBaseRTRenderPass->EndPass();
		char f[30];
		float ff = 1000.0f/(10.0f/FPS_COUNTER_NUMBER);
		sprintf_s(f, "%f", ff);
		GTextDrawer->AddText(TString(f));
		//GTextDrawer->DrawTexts(Viewport);

		GDriver->EndScene(Viewport);
	}    
}

void BRenderingBatchManager::RemovePrimitive(BPrimitive* Primitive) {
	Primitive->Batch->RemovePrimitive(Primitive);
}

void BRenderingBatchManager::AddPrimitive(BPrimitive* Primitive) {
	for(unsigned int i=0;i<Batches.Size();++i) {
		BRenderingBatch* Batch = Batches(i);
		if(Batch->IsBatchable(Primitive)) {
			Batch->BatchPrimitive(Primitive);
			return;
		}
	}
    Batches.AddItem(new BRenderingBatch(Primitive));
}
