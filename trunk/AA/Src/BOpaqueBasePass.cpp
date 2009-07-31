#include "StdAfx.h"

#include "RResource.h"

#include "BOpaqueBasePass.h"
#include "BDriver.h"

BOpaqueBasePass::BOpaqueBasePass()
{
	RRenderTarget *DXRT = GDriver->CreateRenderTarget(800, 800, PF_A8R8G8B8);
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
	RShaderBase* pShader = RShaderTable::pShaders(0);
	for(unsigned int i=0;i<m_RenderTargets.Size();++i)
		GDriver->SetRenderTarget(i, m_RenderTargets(i));
	GDriver->Clear(true, 0x0000FF00);
	pShader->BeginShader();
	pShader->SetParameter(Viewport);
}

void BOpaqueBasePass::EndPass()
{
	RShaderBase* pShader = RShaderTable::pShaders(0);
	pShader->EndShader();
}

void BOpaqueBasePass::DrawPrimitive(TPrimitiveTemplateBase *Prim)
{
	GDriver->DrawPrimitive(Prim->pVertexBuffer->nVertices, Prim->pIndexBuffer->nIndices);
}