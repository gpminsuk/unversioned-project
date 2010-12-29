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

void BOpaqueBasePass::BeginPass(BViewport* InViewport)
{
	__super::BeginPass(InViewport);

	GDriver->SetRenderTarget(0, RPR->m_BaseSceneRT);

	GDriver->Clear(true, 0x00000000, true, 1.0f);

	GDriver->SetVertexDeclaration(VertexType_Position | VertexType_UV | VertexType_Normal);
}

void BOpaqueBasePass::DrawPrimitive(BRenderingBatch *Batch)
{
	if(Batch->PrimitiveType == PrimitiveType_LineList)
		GDriver->DrawPrimitive(Batch->PrimitiveType, Batch->nVertices/2);
	else
		GDriver->DrawIndexedPrimitive(Batch->PrimitiveType, Batch->nVertices, Batch->GetNumIndices());
}