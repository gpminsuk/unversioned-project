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

void BParticleRenderPass::BeginPass(BViewport* Viewport)
{
	m_Viewport = Viewport;

	RShaderBase* pShader = RShaderTable::Shaders(4);
	pShader->BeginShader();
	pShader->SetParameter(Viewport);

	GDriver->SetVertexDeclaration(VertexType_Position | VertexType_UV);
}

void BParticleRenderPass::EndPass()
{
	RShaderBase* pShader = RShaderTable::Shaders(4);
	pShader->EndShader();
}

void BParticleRenderPass::DrawPrimitive(BRenderingBatch *Batch)
{
	GDriver->DrawIndexedPrimitive(Batch->PrimitiveType, Batch->nVertices, Batch->GetNumIndices());
}