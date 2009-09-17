#include "StdAfx.h"

#include "RResource.h"

#include "BOpaqueBasePass.h"
#include "BDriver.h"

BOpaqueBasePass::BOpaqueBasePass()
{
	RRenderTarget *DXRT = GDriver->CreateRenderTarget(1024, 768, PixelFormat_A8R8G8B8);
	m_RenderTargets.AddItem(DXRT);
}

BOpaqueBasePass::~BOpaqueBasePass()
{
	for(unsigned int i=0;i<m_RenderTargets.Size();++i)
	{
		m_RenderTargets(i)->Release();
		delete m_RenderTargets(i);
		m_RenderTargets.DeleteItem(0);
	}
}

void BOpaqueBasePass::BeginPass(BViewport* Viewport)
{
	RShaderBase* pShader = RShaderTable::Shaders(0);
	for(unsigned int i=0;i<m_RenderTargets.Size();++i)
		GDriver->SetRenderTarget(i, m_RenderTargets(i));

	GDriver->Clear(true, 0x00000000);
	pShader->BeginShader();
	pShader->SetParameter(Viewport);	

	GDriver->SetVertexDeclaration(VertexType_Position | VertexType_UV | VertexType_Normal);
}

void BOpaqueBasePass::EndPass()
{
	RShaderBase* pShader = RShaderTable::Shaders(0);
	pShader->EndShader();
}

void BOpaqueBasePass::DrawPrimitive(TBatch *Batch)
{
	if(Batch->RenderType == PrimitiveType_LineList)
		GDriver->DrawPrimitive(Batch->RenderType, Batch->nVertices/2);
	else
		GDriver->DrawIndexedPrimitive(Batch->RenderType, Batch->nVertices, Batch->GetNumIndices());
}