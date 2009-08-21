#include "stdafx.h"
#include "RDXResource.h"

#include "BViewport.h"

#include "CDirectXDriver.h"

bool RDirectXShader::BeginShader()
{
	CDirectXDriver* Driver = dynamic_cast<CDirectXDriver*>(GDriver);
	if(!Driver)
		return false;
	Driver->GetDevice()->SetVertexDeclaration(m_pDecl);
	Driver->GetDevice()->SetVertexShader(m_pVertexShader);
	Driver->GetDevice()->SetPixelShader(m_pPixelShader);
	return true;
}

bool RDirectXShader::SetParameter(BViewport* vp)
{
	CDirectXDriver* Driver = dynamic_cast<CDirectXDriver*>(GDriver);
	if(!Driver)
		return false;
	//임시코드
	D3DXMATRIXA16 Proj;
	D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI/4, 1.0f, 0.0001f, 10000.0f);
	D3DXMATRIXA16 View;
	View._11 = vp->m_ViewMatrix._11; View._21 = vp->m_ViewMatrix._21; View._31 = vp->m_ViewMatrix._31; View._41 = vp->m_ViewMatrix._41;
	View._12 = vp->m_ViewMatrix._12; View._22 = vp->m_ViewMatrix._22; View._32 = vp->m_ViewMatrix._32; View._42 = vp->m_ViewMatrix._42;
	View._13 = vp->m_ViewMatrix._13; View._23 = vp->m_ViewMatrix._23; View._33 = vp->m_ViewMatrix._33; View._43 = vp->m_ViewMatrix._43;
	View._14 = vp->m_ViewMatrix._14; View._24 = vp->m_ViewMatrix._24; View._34 = vp->m_ViewMatrix._34; View._44 = vp->m_ViewMatrix._44;

	//D3DXMatrixLookAtLH(&View, &D3DXVECTOR3(3.0f,3.0f,3.0f), &D3DXVECTOR3(0.0f,0.0f,0.0f), &D3DXVECTOR3(0.0f,1.0f,0.0f));
	D3DXMATRIXA16 ViewProj = View*Proj;

	D3DXMatrixTranspose( &ViewProj, &ViewProj );

	Driver->GetDevice()->SetVertexShaderConstantF(0, (float*)&ViewProj, 4);
	return true;
}

bool RDirectXShader::EndShader()
{
	CDirectXDriver* Driver = dynamic_cast<CDirectXDriver*>(GDriver);
	if(!Driver)
		return false;
	Driver->GetDevice()->SetVertexDeclaration(NULL);
	Driver->GetDevice()->SetPixelShader(NULL);
	Driver->GetDevice()->SetVertexShader(NULL);
	return true;
}

bool RDXRenderTarget::Release()
{
	if(m_pRTSurface)
	{
		m_pRTSurface->Release();
		return true;
	}
	return false;	
}

RDXRenderTarget::RDXRenderTarget()
: m_pRTSurface(0)
{
	m_pTexture = new RDXTextureBuffer();
}

RDXRenderTarget::~RDXRenderTarget()
{
	if(m_pRTSurface)
		m_pRTSurface->Release();
	delete m_pTexture;
}

RDXTextureBuffer::~RDXTextureBuffer()
{
	if(m_pTexture)
		m_pTexture->Release();
}

void RDXDynamicPrimitiveBuffer::Release()
{
	RDXVideoMemoryVertexBuffer* DXVB = dynamic_cast<RDXVideoMemoryVertexBuffer*>(m_pVB);
	RDXVideoMemoryIndexBuffer* DXIB = dynamic_cast<RDXVideoMemoryIndexBuffer*>(m_pIB);

	DXVB->VB->Release();
	if(DXIB->nIndices)
		DXIB->IB->Release();
}

RDXDynamicPrimitiveBuffer::RDXDynamicPrimitiveBuffer()
{
	m_pVB = new RDXVideoMemoryVertexBuffer();
	m_pIB = new RDXVideoMemoryIndexBuffer();
}

RDXDynamicPrimitiveBuffer::~RDXDynamicPrimitiveBuffer()
{
	delete m_pVB;
	delete m_pIB;
}