#include "StdAfx.h"

#include "RResource.h"

#include "BDirectionalLightPass.h"
#include "BRenderingBatch.h"
#include "BDriver.h"

BDirectionalLightPass::BDirectionalLightPass()
{
}

BDirectionalLightPass::~BDirectionalLightPass()
{
}

void BDirectionalLightPass::DrawPrimitive(BRenderingBatch *Batch)
{
	if(Batch->PrimitiveType == PrimitiveType_LineList)
		GDriver->DrawPrimitive(Batch->PrimitiveType, Batch->nVertices/2);
	else
		GDriver->DrawIndexedPrimitive(Batch->PrimitiveType, Batch->nVertices, Batch->GetNumIndices());
}

void BDirectionalLightPass::BeginRenderLight(BLightComponent *InLight)
{
	Shader->SetLightParameter(InLight);
}