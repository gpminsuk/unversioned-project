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
	BackBuffer = new RDXRenderTarget();
}

CDirectXDriver::~CDirectXDriver()
{
	delete BackBuffer;
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
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
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

bool CDirectXDriver::SetTexture(int nStage, RTextureBuffer* pTexture)
{
	RDXTextureBuffer* pDXTexture = dynamic_cast<RDXTextureBuffer*>(pTexture);
	if(!pDXTexture)
		return false;
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

	m_pDevice->SetTexture(0,pDXTexture->m_pTexture);
	return true;
}

RDynamicPrimitiveBuffer* CDirectXDriver::CreatePrimitiveBuffer(TBatch* pBatch)
{
	if(!pBatch->m_pTemplates.Size())
		return false;
	RDXDynamicPrimitiveBuffer* PB = new RDXDynamicPrimitiveBuffer();
	RDXVideoMemoryVertexBuffer* VB = dynamic_cast<RDXVideoMemoryVertexBuffer*>(PB->m_pVB);
	RDXVideoMemoryIndexBuffer* IB = dynamic_cast<RDXVideoMemoryIndexBuffer*>(PB->m_pIB);
	if(!VB || !IB)
	{
		return 0;
	}	
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

	hr = m_pDevice->SetStreamSource(0, VB->VB, 0, pBatch->nVertexStride);
	if(hr != D3D_OK)
		return false;

	hr = m_pDevice->CreateIndexBuffer(
		pBatch->nIndices*sizeof(TIndex16),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&IB->IB,
		NULL);

	hr = IB->IB->Lock(0, pBatch->nIndices*sizeof(TIndex16), (void**)&pData, D3DLOCK_DISCARD);
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
		IB->IB->Unlock();
	}

	hr = m_pDevice->SetIndices(IB->IB);
	if(hr != D3D_OK)
		return false;

	return PB;
}

RTextureBuffer* CDirectXDriver::CreateTextureBuffer()
{
	RDXTextureBuffer* TB = new RDXTextureBuffer();

	if(FAILED(D3DXCreateTextureFromFile(m_pDevice, "../Resources/texture.bmp", &TB->m_pTexture)))
		return 0;
	return TB;
}

bool CDirectXDriver::DrawPrimitive(UINT NumVertices, UINT PrimCount)
{
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NumVertices, 0, PrimCount);
	return true;
}

bool CDirectXDriver::DrawPrimitiveUP(UINT NumVertices, UINT PrimCount, PVOID pIndices, UINT IndexStride, PVOID pVertices, UINT VertexStride)
{
	m_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, NumVertices, PrimCount, pIndices, (IndexStride == sizeof(WORD))? D3DFMT_INDEX16 : D3DFMT_INDEX32, pVertices, VertexStride);
	return true;
}

bool CDirectXDriver::Clear(bool bClearColor, DWORD Color, bool bClearDepth, float Depth, bool bClearStencil, DWORD Stencil)
{
	DWORD Flags = 0;

	if(bClearColor)
	{
		Flags |= D3DCLEAR_TARGET;
	}
	if(bClearDepth)
	{
		Flags |= D3DCLEAR_ZBUFFER;
	}
	if(bClearStencil)
	{
		Flags |= D3DCLEAR_STENCIL;
	}

	return m_pDevice->Clear(0, NULL, Flags, Color, Depth, Stencil) == D3D_OK;
	
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

	char FN[256];
	sprintf_s(FN, 256, "..\\Shaders\\Vertex%s", pShader->m_FileName);

	hr = D3DXCompileShaderFromFile(FN, NULL, NULL, "VS", "vs_2_0", dwShaderFlags, &pCode, &pErr, NULL);
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

	sprintf_s(FN, 256, "..\\Shaders\\Pixel%s", pShader->m_FileName);

	hr = D3DXCompileShaderFromFile(FN, NULL, NULL, "PS", "ps_2_0", dwShaderFlags, &pCode, &pErr, NULL);
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
	RDXTextureBuffer *DXTex = dynamic_cast<RDXTextureBuffer*>(DXRT->m_pTexture);
	D3DXCreateTexture(GetDevice(), Width, Height, 1, D3DUSAGE_RENDERTARGET, GPixelFormats[PixelFormat], D3DPOOL_DEFAULT, &DXTex->m_pTexture);
	if(D3D_OK != (DXTex->m_pTexture->GetSurfaceLevel(0, &DXRT->m_pRTSurface)))
		return NULL;
	return DXRT;
}

RRenderTarget* CDirectXDriver::GetBackBuffer()
{	
	GetDevice()->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuffer->m_pRTSurface);
	return BackBuffer;
}

bool CDirectXDriver::SetStreamSource(RDynamicPrimitiveBuffer* PrimitiveBuffer)
{
	RDXVideoMemoryVertexBuffer *DXVertexBuffer = dynamic_cast<RDXVideoMemoryVertexBuffer*>(PrimitiveBuffer->m_pVB);
	if(!DXVertexBuffer)
		return false;
	GetDevice()->SetStreamSource(0, DXVertexBuffer->VB, 0, DXVertexBuffer->nVertexStride);
	return true;
}

bool CDirectXDriver::SetIndices(RDynamicPrimitiveBuffer* PrimitiveBuffer)
{
	RDXVideoMemoryIndexBuffer *DXIndexBuffer = dynamic_cast<RDXVideoMemoryIndexBuffer*>(PrimitiveBuffer->m_pIB);
	if(!DXIndexBuffer)
		return false;
	GetDevice()->SetIndices(DXIndexBuffer->IB);
	return true;
}

bool CDirectXDriver::SetViewport(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height, float MinZ, float MaxZ)
{
	D3DVIEWPORT9 vp;
	vp.X = x;
	vp.Y = y;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinZ = MinZ;
	vp.MaxZ = MaxZ;
	return GetDevice()->SetViewport(&vp) == D3D_OK;
}

bool CDirectXDriver::SetClipRect(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height)
{
	RECT rt;
	rt.left = x;
	rt.right = x+Width;
	rt.top = y;
	rt.bottom = y+Height;
	return GetDevice()->SetScissorRect(&rt) == D3D_OK;
}