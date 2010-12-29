#include "StdAfx.h"

#include "RResource.h"

#include "BParticleRenderPass.h"
#include "BRenderingBatch.h"
#include "BDriver.h"
#include "BViewport.h"

BParticleRenderPass::BParticleRenderPass()
{
}

BParticleRenderPass::~BParticleRenderPass()
{
}

void BParticleRenderPass::DrawPrimitive(BRenderingBatch *Batch)
{
	GDriver->DrawIndexedPrimitive(Batch->PrimitiveType, Batch->nVertices, Batch->GetNumIndices());
}