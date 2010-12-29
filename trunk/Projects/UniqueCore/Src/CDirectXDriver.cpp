#include "StdAfx.h"

#include "BViewport.h"
#include "BPrimitive.h"
#include "BRenderingBatch.h"

#include "CDirectXDriver.h"
#include "CWindowApp.h"

#include "RDXResource.h"

D3DFORMAT GPixelFormats[] = 
{
	D3DFMT_A8R8G8B8,
	D3DFMT_A16B16G16R16,
	D3DFMT_D24S8
};

DWORD GTextureUsage[] = 
{
	D3DUSAGE_RENDERTARGET,
	D3DUSAGE_DEPTHSTENCIL,
	D3DUSAGE_DYNAMIC,
};

DWORD GFillMode[] = 
{
	D3DFILL_POINT,
	D3DFILL_WIREFRAME,
	D3DFILL_SOLID,
};

DWORD GPrimitiveType[] =
{
	D3DPT_POINTLIST,
	D3DPT_LINELIST,
	D3DPT_LINESTRIP,
	D3DPT_TRIANGLELIST,
	D3DPT_TRIANGLESTRIP,
	D3DPT_TRIANGLEFAN
};

DWORD GStencilOperation[] =
{
	D3DSTENCILOP_KEEP,
	D3DSTENCILOP_ZERO,
	D3DSTENCILOP_REPLACE,
	D3DSTENCILOP_INCRSAT,
	D3DSTENCILOP_DECRSAT,
	D3DSTENCILOP_INVERT,
	D3DSTENCILOP_INCR,
	D3DSTENCILOP_DECR
};

DWORD GBlendState[] = 
{
	D3DBLEND_ZERO,
	D3DBLEND_ONE,
	D3DBLEND_SRCCOLOR,
	D3DBLEND_INVSRCCOLOR,
	D3DBLEND_SRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_DESTALPHA, 
	D3DBLEND_INVDESTALPHA,
	D3DBLEND_DESTCOLOR,
	D3DBLEND_INVDESTCOLOR,
	D3DBLEND_SRCALPHASAT,
	D3DBLEND_BOTHSRCALPHA,
	D3DBLEND_BOTHINVSRCALPHA,
	D3DBLEND_BLENDFACTOR,
	D3DBLEND_INVBLENDFACTOR,
	D3DBLEND_SRCCOLOR2,
	D3DBLEND_INVSRCCOLOR2
};

DWORD GCompareFunction[] = 
{
	D3DCMP_NEVER,
	D3DCMP_LESS,
	D3DCMP_EQUAL,
	D3DCMP_LESSEQUAL,
	D3DCMP_GREATER,
	D3DCMP_NOTEQUAL,
	D3DCMP_GREATEREQUAL,
	D3DCMP_ALWAYS,
};

DWORD GCullMode[] =
{
	D3DCULL_NONE,
	D3DCULL_CW,
	D3DCULL_CCW
};

CDirectXDriver::CDirectXDriver(TDXWindowInfo Window)
:	m_pWindow(Window)
{
	BackBuffer = new RDXRenderTarget();
	for(int i=0;i<VertexType_End;++i)
		m_Declarations[i] = 0;
}

CDirectXDriver::~CDirectXDriver()
{
	for(int i=0;i<VertexType_End;++i)
		if(m_Declarations[i])
			m_Declarations[i]->Release();
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
	Parameters.BackBufferWidth = m_pWindow.m_wWidth;
	Parameters.BackBufferHeight = m_pWindow.m_wHeight;
	Parameters.BackBufferCount = 1;
	Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;

	Parameters.EnableAutoDepthStencil = TRUE;
	Parameters.AutoDepthStencilFormat = D3DFMT_D16;
	Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//Parameters.PresentationInterval = D3DPRESENT_DONOTWAIT;	

	HRESULT hResult;
	if((hResult = m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_pWindow.m_hWnd,
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

	CurrentCullMode = CullMode_CCW;

	GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, CurrentDepthState.EnableDepthWrite);
	GetDevice()->SetRenderState(D3DRS_ZFUNC, GCompareFunction[CurrentDepthState.DepthTest]);
	GetDevice()->SetRenderState(D3DRS_CULLMODE, GCullMode[CurrentCullMode]);
	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

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

RDynamicPrimitiveBuffer* CDirectXDriver::CreatePrimitiveBuffer(BRenderingBatch* pBatch)
{
	if(!pBatch->Primitives.Size())
		return false;

	for(int i=0;i<(int)pBatch->Primitives.Size();++i)
	{
		BPrimitive* Prim = pBatch->Primitives(i);
		Prim->UpdatePrimitive();
	}
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
		default:
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
		default:
			return false;
		}
	}
	else
	{
		char* pcData = static_cast<char*>(pData);
		for(int i=0;i<(int)pBatch->Primitives.Size();++i)
		{
			BPrimitive* Prim = pBatch->Primitives(i);
 			Prim->FillDynamicVertexBuffer(&pcData);
		}
		VB->VB->Unlock();
	}

	hr = m_pDevice->SetStreamSource(0, VB->VB, 0, pBatch->nVertexStride);
	if(hr != D3D_OK)
		return false;

	pBatch->IndexTessellate();

	IB->nIndices = pBatch->GetNumIndices();
	if(IB->nIndices)
	{
		hr = m_pDevice->CreateIndexBuffer(
			IB->nIndices*sizeof(TIndex16),
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&IB->IB,
			NULL);

		hr = IB->IB->Lock(0, IB->nIndices*sizeof(TIndex16), (void**)&pData, D3DLOCK_DISCARD);
		if(hr != D3D_OK)
		{
			return false;
		}
		else
		{
			TIndex16* pcData = static_cast<TIndex16*>(pData);
			unsigned short BaseIdx = 0;
			for(int i=0;i<(int)pBatch->Primitives.Size();++i)
			{
				BPrimitive* Prim = pBatch->Primitives(i);
				Prim->FillDynamicIndexBuffer(&pcData, &BaseIdx);
			}
			IB->IB->Unlock();
		}

		hr = m_pDevice->SetIndices(IB->IB);
	}
	if(hr != D3D_OK)
		return false;

	return PB;
}

RTextureBuffer* CDirectXDriver::CreateTextureBuffer(const TString& Str)
{
	RDXTextureBuffer* TB = new RDXTextureBuffer();

	TCHAR Chr[1024];
	MultiByteToWideChar(CP_ACP,0,Str.Str,1024,Chr,1024);
	//wsprintf(Chr, TEXT("%s"), Str.Str);
	if(FAILED(D3DXCreateTextureFromFile(m_pDevice, Chr, &TB->m_pTexture)))
	{
		delete TB;
		return 0;
	}
	return TB;
}

bool CDirectXDriver::DrawIndexedPrimitive(EPrimitiveType PrimitiveType, UINT NumVertices, UINT PrimCount)
{
	m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)GPrimitiveType[PrimitiveType], 0, 0, NumVertices, 0, PrimCount);
	return true;
}

bool CDirectXDriver::DrawPrimitive(EPrimitiveType PrimitiveType, UINT PrimCount)
{
	m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE)GPrimitiveType[PrimitiveType], 0, PrimCount);
	return true;
}

bool CDirectXDriver::DrawIndexedPrimitiveUP(EPrimitiveType PrimitiveType, UINT NumVertices, UINT PrimCount, PVOID pIndices, UINT IndexStride, PVOID pVertices, UINT VertexStride)
{
	m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)GPrimitiveType[PrimitiveType], 0, NumVertices, PrimCount, pIndices, (IndexStride == sizeof(WORD))? D3DFMT_INDEX16 : D3DFMT_INDEX32, pVertices, VertexStride);
	return true;
}

bool CDirectXDriver::DrawPrimitiveUP(EPrimitiveType PrimitiveType, unsigned int NumVertices, void* pVertices, unsigned int VertexStride)
{
	m_pDevice->DrawPrimitiveUP((D3DPRIMITIVETYPE)GPrimitiveType[PrimitiveType], NumVertices, pVertices, VertexStride);
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
 
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pErr = NULL;
	DWORD dwShaderFlags = 0;

	WCHAR FN[256];
	
	//sprintf_s(FN, 256, "..\\Shaders\\Vertex%s", pShader->m_FileName);
	// TODO : 임시코드
	static int Cnt = 0;	
	if(Cnt == 0)
	{
		wsprintf(FN, TEXT("..\\..\\Shaders\\BaseSceneVertexShader.uvs"));
		FILE* fp = NULL;
		FILE* cachefp = NULL;
		WCHAR CacheFN[256];
		wsprintf(CacheFN, TEXT("..\\..\\Shaders\\Cache\\StaticMeshBaseSceneVertexShader.uvs"));
		_wfopen_s(&cachefp, CacheFN, TEXT("w+"));
		_wfopen_s(&fp, FN, TEXT("r"));
		if(fp)
		{
			if(cachefp)
			{
				char syntax[256];
				while(!feof(fp))
				{
					fgets(syntax, 256, fp);
					char* syntaxTmp;
					if(syntaxTmp = strstr(syntax, "#include"))
					{
						if(syntaxTmp = strstr(syntax, "\""))
						{
							syntaxTmp += 1;
							char rest[256];
							sprintf_s(rest, "%s", syntaxTmp);
							syntaxTmp[0] = '\0';
							sprintf_s(syntax, 256, "%s..\\%s", syntax, rest);
						}
					}

					if(syntaxTmp = strstr(syntax, "VertexProtocol.ush"))
					{
						char rest[256];
						char replace[256];
						sprintf_s(rest, "%s", syntaxTmp);
						syntaxTmp[0] = '\0';
						sprintf_s(replace, 256, "%sStaticMesh%s", syntax, rest);
						fprintf_s(cachefp, "%s", replace);
					}
					else
					{
						fprintf_s(cachefp, "%s", syntax);
					}
				}
				fclose(fp);
				fclose(cachefp);
			}
			else
			{
				fclose(fp);
				return false;
			}
		}
		else
		{
			return false;
		}
		wsprintf(FN, TEXT("%s"), CacheFN);
	}
	else
	{
		wsprintf(FN, TEXT("..\\..\\Shaders\\Vertex%s"), pShader->m_FileName);
	}	

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

	//sprintf_s(FN, 256, "..\\Shaders\\Pixel%s", pShader->m_FileName);
	if(Cnt == 0)
	{
		wsprintf(FN, TEXT("..\\..\\Shaders\\BaseScenePixelShader.ups"));
	}
	else
	{
		wsprintf(FN, TEXT("..\\..\\Shaders\\Pixel%s"), pShader->m_FileName);
	}

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

	Cnt++;
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

bool CDirectXDriver::SetDepthStencilSurface(RRenderTarget* RT)
{
	RDXRenderTarget *DXRT = dynamic_cast<RDXRenderTarget*>(RT);
	if(!DXRT)
		return false;
	GetDevice()->SetDepthStencilSurface(DXRT->m_pRTSurface);
	return true;
}

RRenderTarget* CDirectXDriver::CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat, ETextureUsage TexUsage)
{
	RDXRenderTarget *DXRT = new RDXRenderTarget();
	RDXTextureBuffer *DXTex = dynamic_cast<RDXTextureBuffer*>(DXRT->m_pTexture);
	D3DXCreateTexture(GetDevice(), Width, Height, 1, GTextureUsage[TexUsage], GPixelFormats[PixelFormat], D3DPOOL_DEFAULT, &DXTex->m_pTexture);
	if(D3D_OK != (DXTex->m_pTexture->GetSurfaceLevel(0, &DXRT->m_pRTSurface)))
		return NULL;
	DXRT->m_SizeX = Width;
	DXRT->m_SizeY = Height;
	return DXRT;
}

RTextureBuffer* CDirectXDriver::CreateTextureBuffer(unsigned int Width, unsigned int Height)
{
	RDXTextureBuffer* DXTex = new RDXTextureBuffer();	
	D3DXCreateTexture(GetDevice(), Width, Height, 1, GTextureUsage[TexUsage_Dynamic], GPixelFormats[PixelFormat_A8R8G8B8], D3DPOOL_DEFAULT, &DXTex->m_pTexture);
	DXTex->Width = Width;
	DXTex->Height = Height;
	return DXTex;
}

RTextureBuffer* CDirectXDriver::CreateFontBuffer(unsigned int Width, unsigned int Height)
{
	RDXFontBuffer* DXTex = new RDXFontBuffer();	
	D3DXCreateTexture(GetDevice(), Width, Height, 1, GTextureUsage[TexUsage_Dynamic], GPixelFormats[PixelFormat_A8R8G8B8], D3DPOOL_DEFAULT, &DXTex->m_pTexture);
	DXTex->Width = Width;
	DXTex->Height = Height;
	return DXTex;
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

bool CDirectXDriver::SetVertexDeclaration(unsigned long Type)
{
	if(!m_Declarations[Type])
	{
		D3DVERTEXELEMENT9 Decl[MAX_FVF_DECL_SIZE];
		unsigned long FVF = 0;
		unsigned int TexCount = 0;

		if(Type & VertexType_Position)
		{
			FVF |= D3DFVF_XYZ;
		}
		if(Type & VertexType_UV)
		{
			FVF |= D3DFVF_TEX1;
			FVF |= D3DFVF_TEXCOORDSIZE2(TexCount++);
		}
		if(Type & VertexType_Normal)
		{
			FVF |= D3DFVF_NORMAL;
		}

		D3DXDeclaratorFromFVF(FVF, Decl);
		GetDevice()->CreateVertexDeclaration(Decl, &m_Declarations[Type]);
	}
	GetDevice()->SetVertexDeclaration(m_Declarations[Type]);
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

void CDirectXDriver::SetFillMode(EFillMode FM)
{
	if(CurrentFillMode != FM)
	{
		GetDevice()->SetRenderState(D3DRS_FILLMODE,GFillMode[CurrentFillMode = FM]);
	}
}

void CDirectXDriver::SetStencilState(TStencilState& StencilState)
{
	if(CurrentStencilState.EnableStencil != StencilState.EnableStencil)
	{
		GetDevice()->SetRenderState(D3DRS_STENCILENABLE, CurrentStencilState.EnableStencil = StencilState.EnableStencil);
	}
	if(CurrentStencilState.EnableStencil)
	{
		bool DoesUseComp=false;
		if(CurrentStencilState.StencilTest != StencilState.StencilTest)
		{
			GetDevice()->SetRenderState(D3DRS_STENCILFUNC, GCompareFunction[CurrentStencilState.StencilTest = StencilState.StencilTest]);
		}
		if(CurrentStencilState.StencilTest != CompareFunc_Always || CurrentStencilState.StencilTest != CompareFunc_Never)
		{
			if(CurrentStencilState.StencilFailOp != StencilState.StencilFailOp)
			{
				GetDevice()->SetRenderState(D3DRS_STENCILFAIL, GStencilOperation[CurrentStencilState.StencilFailOp = StencilState.StencilFailOp]);
			}
			if(CurrentStencilState.StencilZFailOp != StencilState.StencilZFailOp)
			{
				GetDevice()->SetRenderState(D3DRS_STENCILZFAIL, GStencilOperation[CurrentStencilState.StencilZFailOp = StencilState.StencilZFailOp]);
			}
			if(CurrentStencilState.StencilZPassOp != StencilState.StencilZPassOp)
			{
				GetDevice()->SetRenderState(D3DRS_STENCILPASS, GStencilOperation[CurrentStencilState.StencilZPassOp = StencilState.StencilZPassOp]);
			}
			DoesUseComp = true;
		}
		if(CurrentStencilState.EnableTwoSideStencil != StencilState.EnableTwoSideStencil)
		{
			GetDevice()->SetRenderState(D3DRS_CCW_STENCILFUNC, GCompareFunction[CurrentStencilState.StencilTest = StencilState.StencilTest]);
		}
		if(CurrentStencilState.EnableTwoSideStencil)
		{
			if(CurrentStencilState.BackFaceStencilTest != StencilState.BackFaceStencilTest)
			{
				GetDevice()->SetRenderState(D3DRS_CCW_STENCILFUNC, GCompareFunction[CurrentStencilState.BackFaceStencilTest = StencilState.BackFaceStencilTest]);
			}
			if(CurrentStencilState.BackFaceStencilTest != CompareFunc_Always || CurrentStencilState.BackFaceStencilTest != CompareFunc_Never)
			{
				if(CurrentStencilState.BackFaceStencilFailOp != StencilState.BackFaceStencilFailOp)
				{
					GetDevice()->SetRenderState(D3DRS_CCW_STENCILFAIL, GStencilOperation[CurrentStencilState.BackFaceStencilFailOp = StencilState.BackFaceStencilFailOp]);
				}
				if(CurrentStencilState.BackFaceStencilZFailOp != StencilState.BackFaceStencilZFailOp)
				{
					GetDevice()->SetRenderState(D3DRS_CCW_STENCILZFAIL, GStencilOperation[CurrentStencilState.BackFaceStencilZFailOp = StencilState.BackFaceStencilZFailOp]);
				}
				if(CurrentStencilState.BackFaceStencilZPassOp != StencilState.BackFaceStencilZPassOp)
				{
					GetDevice()->SetRenderState(D3DRS_CCW_STENCILPASS, GStencilOperation[CurrentStencilState.BackFaceStencilZPassOp = StencilState.BackFaceStencilZPassOp]);
				}
				DoesUseComp = true;
			}
		}
		if(DoesUseComp)
		{
			if(CurrentStencilState.StencilReadMask != StencilState.StencilReadMask)
			{
				GetDevice()->SetRenderState(D3DRS_STENCILMASK, CurrentStencilState.StencilReadMask = StencilState.StencilReadMask);
			}
			if(CurrentStencilState.StencilWriteMask != StencilState.StencilWriteMask)
			{
				GetDevice()->SetRenderState(D3DRS_STENCILWRITEMASK, CurrentStencilState.StencilWriteMask = StencilState.StencilWriteMask);
			}
			if(CurrentStencilState.StencilRef != CurrentStencilState.StencilRef)
			{
				GetDevice()->SetRenderState(D3DRS_STENCILREF, CurrentStencilState.StencilRef = StencilState.StencilRef);
			}
		}
	}
}

void CDirectXDriver::SetBlendState(TBlendState& BlendState)
{
	if(CurrentBlendState.SrcBlendState != BlendState.SrcBlendState)
	{
		GetDevice()->SetRenderState(D3DRS_SRCBLEND, GBlendState[CurrentBlendState.SrcBlendState = BlendState.SrcBlendState]);
	}
	if(CurrentBlendState.DestBlendState != BlendState.DestBlendState)
	{
		GetDevice()->SetRenderState(D3DRS_DESTBLEND, GBlendState[CurrentBlendState.DestBlendState = BlendState.DestBlendState]);
	}
}

void CDirectXDriver::SetDepthState(TDepthState& DepthState)
{
	if(CurrentDepthState.EnableDepthWrite != DepthState.EnableDepthWrite)
	{
		GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, CurrentDepthState.EnableDepthWrite = DepthState.EnableDepthWrite);
	}
	if(CurrentDepthState.EnableDepthWrite)
	{
		if(CurrentDepthState.DepthTest != DepthState.DepthTest)
		{
			GetDevice()->SetRenderState(D3DRS_ZFUNC, CurrentDepthState.DepthTest = DepthState.DepthTest);
		}
	}
}

void CDirectXDriver::SetCullMode(ECullMode& CullMode)
{
	if(CurrentCullMode != CullMode)
	{
		GetDevice()->SetRenderState(D3DRS_CULLMODE, CullMode);
	}
}