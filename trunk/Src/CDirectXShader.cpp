#include "StdAfx.h"

#include "BViewport.h"

#include "CDirectXShader.h"
#include "CDirectXDriver.h"

bool CDirectXShader::CompileShaderFromFile()
{	
	HRESULT hr;
	D3DVERTEXELEMENT9 Decl[MAX_FVF_DECL_SIZE];

	D3DXDeclaratorFromFVF(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0), Decl);
	m_pDirectX->GetDevice()->CreateVertexDeclaration(Decl, &m_pDecl);

	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pErr = NULL;
	DWORD dwShaderFlags = 0;
	hr = D3DXCompileShaderFromFile(".\\Shaders\\VertexShader.fx", NULL, NULL, "VS", "vs_2_0", dwShaderFlags, &pCode, &pErr, NULL);
	if(hr != D3D_OK)
	{
		char Err[1024];
		sprintf_s(Err,1024,"%s",(char*)pErr->GetBufferPointer());
		pErr->Release();
		return false;
	}
	m_pDirectX->GetDevice()->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &m_pVertexShader);
	pCode->Release();
	pCode = NULL;

	hr = D3DXCompileShaderFromFile(".\\Shaders\\PixelShader.fx", NULL, NULL, "PS", "ps_2_0", dwShaderFlags, &pCode, &pErr, NULL);
	if(hr != D3D_OK)
	{
		char Err[1024];
		sprintf_s(Err,1024,"%s",(char*)pErr->GetBufferPointer());
		pErr->Release();
		return false;
	}
	m_pDirectX->GetDevice()->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &m_pPixelShader);
	pCode->Release();
	return true;
}

bool CDirectXShader::AssembleShaderFromFile()
{
	return true;
}

bool CDirectXShader::CompileShaderFromMemory()
{
	return true;
}

bool CDirectXShader::AssembleShaderFromMemory()
{
	return true;
}

bool CDirectXShader::BeginShader()
{
	m_pDirectX->GetDevice()->SetVertexDeclaration(m_pDecl);
	m_pDirectX->GetDevice()->SetVertexShader(m_pVertexShader);
	m_pDirectX->GetDevice()->SetPixelShader(m_pPixelShader);
	return true;
}

bool CDirectXShader::SetParameter(BViewport* vp)
{
	//임시코드
	D3DXMATRIXA16 Proj;
	D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI/2, 1.0f, 1.0f, 100.0f);
	D3DXMATRIXA16 View;
	View._11 = vp->m_ViewMatrix._11; View._21 = vp->m_ViewMatrix._21; View._31 = vp->m_ViewMatrix._31; View._41 = vp->m_ViewMatrix._41;
	View._12 = vp->m_ViewMatrix._12; View._22 = vp->m_ViewMatrix._22; View._32 = vp->m_ViewMatrix._32; View._42 = vp->m_ViewMatrix._42;
	View._13 = vp->m_ViewMatrix._13; View._23 = vp->m_ViewMatrix._23; View._33 = vp->m_ViewMatrix._33; View._43 = vp->m_ViewMatrix._43;
	View._14 = vp->m_ViewMatrix._14; View._24 = vp->m_ViewMatrix._24; View._34 = vp->m_ViewMatrix._34; View._44 = vp->m_ViewMatrix._44;
	
	//D3DXMatrixLookAtLH(&View, &D3DXVECTOR3(3.0f,3.0f,3.0f), &D3DXVECTOR3(0.0f,0.0f,0.0f), &D3DXVECTOR3(0.0f,1.0f,0.0f));
	D3DXMATRIXA16 ViewProj = View*Proj;

	D3DXMatrixTranspose( &ViewProj, &ViewProj );

	m_pDirectX->GetDevice()->SetVertexShaderConstantF(0, (float*)&ViewProj, 4);
	return true;
}

bool CDirectXShader::EndShader()
{
	m_pDirectX->GetDevice()->SetPixelShader(NULL);
	m_pDirectX->GetDevice()->SetVertexShader(NULL);
	return true;
}

bool CDirectXShader::BeginPass()
{
	return true;
}

bool CDirectXShader::EndPass()
{
	return true;
}
