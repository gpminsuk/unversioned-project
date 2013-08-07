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

BRenderingBatch::BRenderingBatch(BPrimitive* InitialPrimitive)
    :
    nVertices(0) {
	BatchPrimitive(InitialPrimitive);
}

BRenderingBatch::~BRenderingBatch() {
    Primitives.Clear(true);
}

bool BRenderingBatch::IsBatchable(BPrimitive* Primitive) {	
	return (pMaterial == Primitive->GetMaterial() && RenderType == Primitive->RenderType && Protocol == Primitive->Primitives(0)->pBuffer->m_pVB->Protocol);
}

void BRenderingBatch::RemovePrimitive(BPrimitive* Primitive) {
	for (unsigned int i = 0; i < Primitive->Primitives.Size(); ++i) {
		TPrimitive* Prim = Primitive->Primitives(i);
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
	for (unsigned int i = 0; i < Primitive->Primitives.Size(); ++i) {
		TPrimitive* Prim = Primitive->Primitives(i);
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
    PrimitiveBuffer = GDriver->CreatePrimitiveBuffer(this);
    if (PrimitiveBuffer) {
        switch (RenderType) {
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

void BRenderingBatch::RenderBaseScene() {
    PrimitiveBuffer = GDriver->CreatePrimitiveBuffer(this);
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
        PrimitiveBuffer->Release();
        delete PrimitiveBuffer;
    }
}

BRenderingBatchManager::BRenderingBatchManager() {
}

BRenderingBatchManager::~BRenderingBatchManager() {
}

void BRenderingBatchManager::RenderBatches(BViewport* Viewport) {
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

    GDirectionalLightPass->BeginPass(Viewport);
    for (unsigned int i = 0; i < Viewport->Lights.Size(); ++i) {
        for (unsigned int i = 0; i < Batches.Size(); ++i) {
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
