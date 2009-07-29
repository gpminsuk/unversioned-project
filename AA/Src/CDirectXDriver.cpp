#include "StdAfx.h"

#include "BViewport.h"

#include "CDirectXDriver.h"
#include "CWindowApp.h"

#include "RDXResource.h"

D3DFORMAT GPixelFormats[] = 
{
	D3DFMT_A8R8G8B8
};

CDirectXDriver::CDirectXDriver(TWindowInfo* Window)
:	m_pWindow(Window)
{
}

bool CDirectXDriver::CreateDriver()
{
	if((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		// TODO : Error Handling Code
		return false;
	}

	// TODO : Parameter Settings
	D3DPRESENT_PARAMETERS Parameters;
	ZeroMemory(&Parameters, sizeof(Parameters));

	Parameters.Windowed = true;
	Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Parameters.BackBufferWidth = m_pWindow->m_wWidth;
	Parameters.BackBufferHeight = m_pWindow->m_wHeight;
	Parameters.BackBufferCount = 1;
	Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;

	Parameters.EnableAutoDepthStencil = TRUE;
	Parameters.AutoDepthStencilFormat = D3DFMT_D16;
	//Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hResult;
	if((hResult = m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_pWindow->m_hWnd,
		//D3DCREATE_HARDWARE_VERTEXPROCESSING,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&Parameters,
		&m_pDevice)) != D3D_OK)
	{
		// TODO : Error Handling Code
		switch(hResult)
		{
		case D3DERR_DEVICELOST:
			printf("ERROR");
			break;
		case D3DERR_INVALIDCALL:
			printf("ERROR");
			break;
		case D3DERR_NOTAVAILABLE:
			printf("ERROR");
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			printf("ERROR");
			break;
		}
		return false;
	}

	m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	CompileShader();
	return true;
}

bool CDirectXDriver::DestroyDriver()
{
	if(m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = 0;
	}
	if(m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = 0;
	}
	return true;
}

bool CDirectXDriver::CompileShader()
{
	RShaderTable::nTableSize = 1;
	RShaderTable::pShaders = new RDirectXShader();
	sprintf_s(RShaderTable::pShaders->m_FileName, 256, "Shader.fx");
	for(int i=0;i<RShaderTable::nTableSize;++i)
	{
		GDriver->CompileShaderFromFile(RShaderTable::pShaders);
	}
	return true;
}

bool CDirectXDriver::SetTexture(int nStage, BTextureBuffer* pTexture)
{
	CDirectXTextureBuffer* pDXTexture = dynamic_cast<CDirectXTextureBuffer*>(pTexture);
	if(!pDXTexture)
		return false;
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

	m_pDevice->SetTexture(0,pDXTexture->pTexture);
	return true;
}

BPrimitiveBuffer* CDirectXDriver::CreatePrimitiveBuffer(TBatch* pBatch)
{
	CDirectXVertexBuffer* VB = new CDirectXVertexBuffer();
	HRESULT hr;
	hr = m_pDevice->CreateVertexBuffer(
		pBatch->nVertices * pBatch->nVertexStride,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&VB->VB,
		NULL);
	if(hr != D3D_OK)
	{
		switch(hr)
		{
		case D3DERR_OUTOFVIDEOMEMORY:
			return false;
		}
	}
	void *pData;
	hr = VB->VB->Lock(0, pBatch->nVertices * pBatch->nVertexStride
		, (void**)&pData, D3DLOCK_DISCARD);
	if(hr != D3D_OK)
	{
		switch(hr)
		{
		case D3DERR_INVALIDCALL:
			break;
		}
	}
	else
	{
		char* pcData = static_cast<char*>(pData);
		for(int i=0;i<(int)pBatch->m_pTemplates.Size();++i)
		{			
			memcpy(pcData, pBatch->m_pTemplates[i]->pVertexBuffer->pVertices, 
				pBatch->m_pTemplates[i]->pVertexBuffer->nVertices * pBatch->m_pTemplates[i]->pVertexBuffer->nVertexStride);
			pcData += pBatch->m_pTemplates[i]->pVertexBuffer->nVertices * pBatch->m_pTemplates[i]->pVertexBuffer->nVertexStride;
		}
		VB->VB->Unlock();
	}

	m_pDevice->SetStreamSource(0, VB->VB, 0, pBatch->nVertexStride);

	hr = m_pDevice->CreateIndexBuffer(
		14*sizeof(TIndex16),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&VB->IB,
		NULL);

	hr = VB->IB->Lock(0, 14*sizeof(TIndex16), (void**)&pData, D3DLOCK_DISCARD);
	if(hr != D3D_OK)
	{
		return false;
	}
	else
	{
		TIndex16* pcData = static_cast<TIndex16*>(pData);
		short BaseIdx = 0;
		for(int i=0;i<(int)pBatch->m_pTemplates.Size();++i)
		{
			for(int k=0;k<pBatch->m_pTemplates[i]->pIndexBuffer->nIndices;++k)
			{
				TIndex16 tmpIndex;
				tmpIndex._1 = pBatch->m_pTemplates[i]->pIndexBuffer->pIndices[k]._1 + BaseIdx;
				tmpIndex._2 = pBatch->m_pTemplates[i]->pIndexBuffer->pIndices[k]._2 + BaseIdx;
				tmpIndex._3 = pBatch->m_pTemplates[i]->pIndexBuffer->pIndices[k]._3 + BaseIdx;
				pcData[k] = tmpIndex;
			}
			BaseIdx += pBatch->m_pTemplates[i]->pVertexBuffer->nVertices;
			pcData += pBatch->m_pTemplates[i]->pIndexBuffer->nIndices;
		}
		VB->IB->Unlock();
	}

	m_pDevice->SetIndices(VB->IB);

	return VB;
}

BTextureBuffer* CDirectXDriver::CreateTextureBuffer()
{
	CDirectXTextureBuffer* TB = new CDirectXTextureBuffer();

	if(FAILED(D3DXCreateTextureFromFile(m_pDevice, "../Resources/texture.bmp", &TB->pTexture)))
		return 0;
	return TB;
}

bool CDirectXTextureBuffer::DestroyTextureBuffer()
{
	pTexture->Release();
	return true;
}

bool CDirectXDriver::DrawPrimitive()
{
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 12, 0, 14);
	return true;
}

bool CDirectXVertexBuffer::DestroyVertexBuffer()
{
	IB->Release();
	VB->Release();
	return true;
}

bool CDirectXDriver::BeginScene()
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	return SUCCEEDED(m_pDevice->BeginScene());
}

bool CDirectXDriver::EndScene()
{
	m_pDevice->EndScene();

	m_pDevice->Present(NULL, NULL, NULL, NULL);
	return true;
}

bool CDirectXDriver::CompileShaderFromFile(RShaderBase *pShader)
{	
	RDirectXShader *pDXShader = dynamic_cast<RDirectXShader*>(pShader);
	if(!pDXShader)
		return false;
	HRESULT hr;
	D3DVERTEXELEMENT9 Decl[MAX_FVF_DECL_SIZE];

	D3DXDeclaratorFromFVF(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0), Decl);
	GetDevice()->CreateVertexDeclaration(Decl, &pDXShader->m_pDecl);

	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pErr = NULL;
	DWORD dwShaderFlags = 0;
	hr = D3DXCompileShaderFromFile("..\\Shaders\\VertexShader.fx", NULL, NULL, "VS", "vs_2_0", dwShaderFlags, &pCode, &pErr, NULL);
	if(hr != D3D_OK)
	{
		char Err[1024];
		sprintf_s(Err,1024,"%s",(char*)pErr->GetBufferPointer());
		pErr->Release();
		return false;
	}
	GetDevice()->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &pDXShader->m_pVertexShader);
	pCode->Release();
	pCode = NULL;

	hr = D3DXCompileShaderFromFile("..\\Shaders\\PixelShader.fx", NULL, NULL, "PS", "ps_2_0", dwShaderFlags, &pCode, &pErr, NULL);
	if(hr != D3D_OK)
	{
		char Err[1024];
		sprintf_s(Err,1024,"%s",(char*)pErr->GetBufferPointer());
		pErr->Release();
		return false;
	}
	GetDevice()->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &pDXShader->m_pPixelShader);
	pCode->Release();
	return true;
}

bool CDirectXDriver::AssembleShaderFromFile(RShaderBase *pShader)
{
	return true;
}

bool CDirectXDriver::CompileShaderFromMemory(RShaderBase *pShader)
{
	return true;
}

bool CDirectXDriver::AssembleShaderFromMemory(RShaderBase *pShader)
{
	return true;
}

bool CDirectXDriver::SetRenderTarget(unsigned int Idx, RRenderTarget* RT)
{
	RDXRenderTarget *DXRT = dynamic_cast<RDXRenderTarget*>(RT);
	if(!DXRT)
		return false;
	GetDevice()->SetRenderTarget(Idx, DXRT->m_pRTSurface);
	return true;
}

RRenderTarget* CDirectXDriver::CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat)
{
	RDXRenderTarget *DXRT = new RDXRenderTarget();
	IDirect3DTexture9 *pTexture;
	D3DXCreateTexture(GetDevice(), Width, Height, 1, D3DUSAGE_RENDERTARGET, GPixelFormats[PixelFormat], D3DPOOL_DEFAULT, &pTexture);
	if(D3D_OK != (pTexture->GetSurfaceLevel(0, &DXRT->m_pRTSurface)))
		return NULL;
	return DXRT;
}

RRenderTarget* CDirectXDriver::GetBackBuffer()
{
	static RDXRenderTarget RT;
	GetDevice()->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&RT.m_pRTSurface);
	return &RT;
}