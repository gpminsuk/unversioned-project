#include "StdAfx.h"

#include "RResource.h"

#include "BOpaqueBasePass.h"
#include "BRenderingBatch.h"
#include "BDriver.h"

BOpaqueBasePass::BOpaqueBasePass()
{
}

BOpaqueBasePass::~BOpaqueBasePass()
{
}

void BOpaqueBasePass::DrawPrimitive(BRenderingBatch *Batch)
{
	if(Batch->PrimitiveType == PrimitiveType_LineList)
		GDriver->DrawPrimitive(Batch->PrimitiveType, Batch->nVertices/2);
	else
		GDriver->DrawIndexedPrimitive(Batch->PrimitiveType, Batch->nVertices, Batch->GetNumIndices());
}