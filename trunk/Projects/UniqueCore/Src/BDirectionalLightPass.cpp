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

void BDirectionalLightPass::BeginPass(BViewport* Viewport, BLightComponent* Light)
{
	GDriver->SetBlendState(TBlendState(BlendState_One, BlendState_One));
	RShaderBase* pShader = RShaderTable::Shaders(5);
	GDriver->SetRenderTarget(0, RPR->m_BaseSceneRT);

	pShader->BeginShader();
	pShader->SetParameter(Viewport);
	pShader->SetLightParameter(Light);

	GDriver->SetVertexDeclaration(VertexType_Position | VertexType_UV | VertexType_Normal);	
}

void BDirectionalLightPass::EndPass()
{
	RShaderBase* pShader = RShaderTable::Shaders(5);
	pShader->EndShader();
}

void BDirectionalLightPass::DrawPrimitive(BRenderingBatch *Batch)
{
	if(Batch->PrimitiveType == PrimitiveType_LineList)
		GDriver->DrawPrimitive(Batch->PrimitiveType, Batch->nVertices/2);
	else
		GDriver->DrawIndexedPrimitive(Batch->PrimitiveType, Batch->nVertices, Batch->GetNumIndices());
}