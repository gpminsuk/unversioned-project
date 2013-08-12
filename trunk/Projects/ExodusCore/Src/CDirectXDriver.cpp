#include "StdAfx.h"

#include "BViewport.h"
#include "BPrimitive.h"
#include "BRenderingBatch.h"
#include "BCamera.h"

#include "CWindowsApplication.h"
#include "CDirectXDriver.h"
#include "CWindowsViewport.h"

#include "RDXResource.h"

#define DX9_SET_BLENDSTATE(a1, a2) \
	if(CurrentBlendState.##a2 != BlendState.##a2)\
	{\
	GetDevice()->SetRenderState(##a1, GBlendState[CurrentBlendState.##a2 = BlendState.##a2]);\
	}

#define DX9_SET_STENCILSTATE(a1, a2) \
	if(CurrentStencilState.##a2 != StencilState.##a2)\
	{\
	GetDevice()->SetRenderState(##a1, GBlendState[CurrentStencilState.##a2 = StencilState.##a2]);\
	}

D3DFORMAT GPixelFormats[] = {
    D3DFMT_A8R8G8B8,
    D3DFMT_A16B16G16R16,
    D3DFMT_D24S8
};

DWORD GTextureUsage[] = {
    D3DUSAGE_RENDERTARGET,
    D3DUSAGE_DEPTHSTENCIL,
    D3DUSAGE_DYNAMIC,
};

DWORD GFillMode[] = {
    D3DFILL_POINT,
    D3DFILL_WIREFRAME,
    D3DFILL_SOLID,
};

DWORD GPrimitiveType[] = {
    D3DPT_POINTLIST,
    D3DPT_LINELIST,
    D3DPT_LINESTRIP,
    D3DPT_TRIANGLELIST,
    D3DPT_TRIANGLESTRIP,
    D3DPT_TRIANGLEFAN
};

DWORD GStencilOperation[] = {
    D3DSTENCILOP_KEEP,
    D3DSTENCILOP_ZERO,
    D3DSTENCILOP_REPLACE,
    D3DSTENCILOP_INCRSAT,
    D3DSTENCILOP_DECRSAT,
    D3DSTENCILOP_INVERT,
    D3DSTENCILOP_INCR,
    D3DSTENCILOP_DECR
};

DWORD GBlendState[] = {
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

DWORD GCompareFunction[] = {
    D3DCMP_NEVER,
    D3DCMP_LESS,
    D3DCMP_EQUAL,
    D3DCMP_LESSEQUAL,
    D3DCMP_GREATER,
    D3DCMP_NOTEQUAL,
    D3DCMP_GREATEREQUAL,
    D3DCMP_ALWAYS,
};

DWORD GCullMode[] = {
    D3DCULL_NONE,
    D3DCULL_CW,
    D3DCULL_CCW
};

BYTE GVertexDeclType[] = {
	D3DDECLTYPE_FLOAT1,
	D3DDECLTYPE_FLOAT2,
	D3DDECLTYPE_FLOAT3,
	D3DDECLTYPE_FLOAT4,
	D3DDECLTYPE_UBYTE4,
	D3DDECLTYPE_SHORT2,
	D3DDECLTYPE_SHORT4
};

BYTE GVertexDeclUsage[] = {
	D3DDECLUSAGE_POSITION,
	D3DDECLUSAGE_BLENDWEIGHT,
	D3DDECLUSAGE_BLENDINDICES,
	D3DDECLUSAGE_NORMAL,
	D3DDECLUSAGE_TEXCOORD,
	D3DDECLUSAGE_TANGENT,
	D3DDECLUSAGE_BINORMAL,
	D3DDECLUSAGE_COLOR,
	D3DDECLUSAGE_DEPTH
};

CDirectXDriver::CDirectXDriver() {
}

CDirectXDriver::~CDirectXDriver() {
}

bool CDirectXDriver::CreateDriver(AApplication* Application) {
	CWindowsApplication* WindowsApplication = dynamic_cast<CWindowsApplication*>(Application);
	if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
        // TODO : Error Handling Code
        return false;
    }

	int Width = WindowsApplication->m_WindowInfo.m_wWidth;
	int Height = WindowsApplication->m_WindowInfo.m_wHeight;

    // TODO : Parameter Settings
    D3DPRESENT_PARAMETERS Parameters;
    ZeroMemory(&Parameters, sizeof(Parameters));

    Parameters.Windowed = true;
    Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Parameters.BackBufferWidth = Width;
    Parameters.BackBufferHeight = Height;
    Parameters.BackBufferCount = 1;
    Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;

    Parameters.EnableAutoDepthStencil = TRUE;
    Parameters.AutoDepthStencilFormat = D3DFMT_D16;
    Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    //Parameters.PresentationInterval = D3DPRESENT_DONOTWAIT;

    HRESULT hResult;
    if ((hResult = m_pD3D->CreateDevice(
                       D3DADAPTER_DEFAULT,
                       D3DDEVTYPE_HAL,
					   WindowsApplication->m_WindowInfo.m_hWnd,
                       D3DCREATE_HARDWARE_VERTEXPROCESSING,
                       //D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                       &Parameters,
                       &m_pDevice)) != D3D_OK) {
        // TODO : Error Handling Code
        switch (hResult) {
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
    GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,
                                CurrentDepthState.EnableDepthWrite);
    GetDevice()->SetRenderState(D3DRS_ZFUNC,
                                GCompareFunction[CurrentDepthState.DepthTest]);
    GetDevice()->SetRenderState(D3DRS_CULLMODE, GCullMode[CurrentCullMode]);
    GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	BackBuffer = new RDXRenderTarget();
	GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &dynamic_cast<RDXRenderTarget*>(BackBuffer)->m_pRTSurface);
	return true;
}

bool CDirectXDriver::ResizeBackBuffer(int Width, int Height) {
	// TODO : Parameter Settings
	D3DPRESENT_PARAMETERS Parameters;
	ZeroMemory(&Parameters, sizeof(Parameters));

	Parameters.Windowed = true;
	Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Parameters.BackBufferWidth = Width;
	Parameters.BackBufferHeight = Height;
	Parameters.BackBufferCount = 1;
	Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;

	Parameters.EnableAutoDepthStencil = TRUE;
	Parameters.AutoDepthStencilFormat = D3DFMT_D16;
	Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//Parameters.PresentationInterval = D3DPRESENT_DONOTWAIT;

	HRESULT hr = S_FALSE;
	BackBuffer->Release();
	//while((hr = GetDevice()->Reset(&Parameters)) != D3D_OK);
	hr = GetDevice()->Reset(&Parameters);
	GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &dynamic_cast<RDXRenderTarget*>(BackBuffer)->m_pRTSurface);
	return true;
}

bool CDirectXDriver::DestroyDriver() {
    if (m_pDevice) {
        m_pDevice->Release();
        m_pDevice = 0;
    }
    if (m_pD3D) {
        m_pD3D->Release();
        m_pD3D = 0;
    }
    return true;
}

bool CDirectXDriver::SetTexture(int nStage, RTextureBuffer* pTexture) {
    RDXTextureBuffer* pDXTexture = dynamic_cast<RDXTextureBuffer*>(pTexture);
    if (!pDXTexture)
        return false;
    m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    HRESULT hr = m_pDevice->SetTexture(0, pDXTexture->m_pTexture);
    return true;
}

RDynamicPrimitiveBuffer* CDirectXDriver::CreatePrimitiveBuffer(BRenderingBatch* pBatch) {
    if (!pBatch->Primitives.Size())
        return false;

    for (int i = 0; i < (int) pBatch->Primitives.Size(); ++i) {
        BPrimitive* Prim = pBatch->Primitives(i);
        Prim->UpdatePrimitive();
    }
    RDXDynamicPrimitiveBuffer* PB = new RDXDynamicPrimitiveBuffer();
    RDXVideoMemoryVertexBuffer* VB =
        dynamic_cast<RDXVideoMemoryVertexBuffer*>(PB->m_pVB);
    RDXVideoMemoryIndexBuffer* IB = dynamic_cast<RDXVideoMemoryIndexBuffer*>(PB
                                    ->m_pIB);
    if (!VB || !IB) {
        return 0;
    }
    HRESULT hr;
    hr = m_pDevice->CreateVertexBuffer(
             pBatch->nVertices * pBatch->Protocol->Decl->GetStride(),
             D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
             0,
             D3DPOOL_DEFAULT,
             &VB->VB,
             NULL);
    if (hr != D3D_OK) {
        switch (hr) {
        case D3DERR_OUTOFVIDEOMEMORY:
            return false;
        default:
            return false;
        }
    }
    void *pData;
    hr = VB->VB->Lock(0, pBatch->nVertices * pBatch->Protocol->Decl->GetStride()
                      , (void**) &pData, D3DLOCK_DISCARD);

    if (hr != D3D_OK) {
        switch (hr) {
        case D3DERR_INVALIDCALL:
            break;
        default:
            return false;
        }
    } else {
        char* pcData = static_cast<char*>(pData);
        for (int i = 0; i < (int) pBatch->Primitives.Size(); ++i) {
            BPrimitive* Prim = pBatch->Primitives(i);
            Prim->FillDynamicVertexBuffer(&pcData);
        }
        VB->VB->Unlock();
    }

    hr = m_pDevice->SetStreamSource(0, VB->VB, 0, pBatch->Protocol->Decl->GetStride());
    if (hr != D3D_OK)
        return false;

    pBatch->IndexTessellate();

    IB->nIndices = pBatch->GetNumIndices();
    if (IB->nIndices) {
        hr = m_pDevice->CreateIndexBuffer(
                 IB->nIndices * sizeof(TIndex16),
                 D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
                 D3DFMT_INDEX16,
                 D3DPOOL_DEFAULT,
                 &IB->IB,
                 NULL);

        hr = IB->IB->Lock(0, IB->nIndices * sizeof(TIndex16), (void**) &pData,
                          D3DLOCK_DISCARD);
        if (hr != D3D_OK) {
            return false;
        } else {
            TIndex16* pcData = static_cast<TIndex16*>(pData);
            unsigned short BaseIdx = 0;
            for (int i = 0; i < (int) pBatch->Primitives.Size(); ++i) {
                BPrimitive* Prim = pBatch->Primitives(i);
                Prim->FillDynamicIndexBuffer(&pcData, &BaseIdx);
            }
            IB->IB->Unlock();
        }

        hr = m_pDevice->SetIndices(IB->IB);
    }
    if (hr != D3D_OK)
        return false;

    return PB;
}

RTextureBuffer* CDirectXDriver::CreateTextureBuffer(const TString& Str) {
    RDXTextureBuffer* TB = new RDXTextureBuffer();

    TCHAR Chr[1024];
    MultiByteToWideChar(CP_ACP, 0, Str.Str, 1024, Chr, 1024);
    //wsprintf(Chr, TEXT("%s"), Str.Str);
    if (FAILED(D3DXCreateTextureFromFile(m_pDevice, Chr, &TB->m_pTexture))) {
        delete TB;
        return 0;
    }
    return TB;
}

bool CDirectXDriver::DrawIndexedPrimitive(EPrimitiveType PrimitiveType, UINT NumVertices, UINT PrimCount) {
    m_pDevice->DrawIndexedPrimitive(
        (D3DPRIMITIVETYPE) GPrimitiveType[PrimitiveType], 0, 0, NumVertices,
        0, PrimCount);
    return true;
}

bool CDirectXDriver::DrawPrimitive(EPrimitiveType PrimitiveType, UINT PrimCount) {
    m_pDevice->DrawPrimitive((D3DPRIMITIVETYPE) GPrimitiveType[PrimitiveType],
                             0, PrimCount);
    return true;
}

bool CDirectXDriver::DrawIndexedPrimitiveUP(EPrimitiveType PrimitiveType, UINT NumVertices, UINT PrimCount, PVOID pIndices, UINT IndexStride, PVOID pVertices, UINT VertexStride) {
    m_pDevice->DrawIndexedPrimitiveUP(
        (D3DPRIMITIVETYPE) GPrimitiveType[PrimitiveType], 0, NumVertices,
        PrimCount, pIndices,
        (IndexStride == sizeof(WORD)) ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
        pVertices, VertexStride);
    return true;
}

bool CDirectXDriver::DrawPrimitiveUP(EPrimitiveType PrimitiveType, unsigned int NumVertices, void* pVertices, unsigned int VertexStride) {
    m_pDevice->DrawPrimitiveUP((D3DPRIMITIVETYPE) GPrimitiveType[PrimitiveType],
                               NumVertices, pVertices, VertexStride);
    return true;
}

bool CDirectXDriver::Clear(bool bClearColor, DWORD Color, bool bClearDepth, float Depth, bool bClearStencil, DWORD Stencil) {
    DWORD Flags = 0;

    if (bClearColor) {
        Flags |= D3DCLEAR_TARGET;
    }
    if (bClearDepth) {
        Flags |= D3DCLEAR_ZBUFFER;
    }
    if (bClearStencil) {
        Flags |= D3DCLEAR_STENCIL;
    }

    return m_pDevice->Clear(0, NULL, Flags, Color, Depth, Stencil) == D3D_OK;

}

bool CDirectXDriver::BeginScene(BViewport* Viewport) {
	RDXRenderTarget* DXBackBuffer = dynamic_cast<RDXRenderTarget*>(BackBuffer);
	GetDevice()->SetRenderTarget(0, DXBackBuffer->m_pRTSurface);
    m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

    return SUCCEEDED(m_pDevice->BeginScene());
}

bool CDirectXDriver::EndScene(BViewport* Viewport) {
	CWindowsViewport* WindowsViewport = dynamic_cast<CWindowsViewport*>(Viewport);
	m_pDevice->EndScene();
	if(WindowsViewport->Handle) {
		m_pDevice->Present(NULL, NULL, WindowsViewport->Handle, NULL);
	}
	return true;
}

bool CDirectXDriver::CompileMaterial(RMaterial* Material) {
	for(unsigned int i=0;i<RShaderPass::ShaderPasses.Size();++i) {
		RShaderPass* ShaderPass = RShaderPass::ShaderPasses(i);
		for(unsigned int j=0;j<RVertexProtocol::Protocols.Size();++j) {
			RVertexProtocol* VertexProtocol = RVertexProtocol::Protocols(j);
			if(Material->VertexShaderFileNames(i*RShaderPass::ShaderPasses.Size() + j) == "") {
				continue;
			}
			RDirectXShader* Shader = new RDirectXShader();
			Material->Shaders.AddItem(Shader);
			Shader->VertexShader = new RDirectXVertexShader();
			Shader->PixelShader = new RDirectXPixelShader();

			RDirectXVertexShader* pDXVertexShader = dynamic_cast<RDirectXVertexShader*>(Shader->VertexShader);
			RDirectXPixelShader* pDXPixelShader = dynamic_cast<RDirectXPixelShader*>(Shader->PixelShader);

			LPD3DXBUFFER pCode = NULL;
			LPD3DXBUFFER pErr = NULL;
			DWORD dwShaderFlags = 0;

			WCHAR FN[256];
			TCHAR Chr[1024];
			MultiByteToWideChar(CP_ACP, 0, Material->VertexShaderFileNames(i*RShaderPass::ShaderPasses.Size() + j).GetBuffer(), 1024, Chr, 1024);
			wsprintf(FN, TEXT("%s"), Chr);

			LPD3DXCONSTANTTABLE ConstantTable;
			HRESULT hr = D3DXCompileShaderFromFile(FN, NULL, NULL, "VS", "vs_2_0",
				dwShaderFlags, &pCode, &pErr, &ConstantTable);
 			if (hr != D3D_OK) {
				char Err[1024];
				sprintf_s(Err, 1024, "%s", (char*) pErr->GetBufferPointer());
				pErr->Release();
				return false;
			}
			GetDevice()->CreateVertexShader((DWORD*) pCode->GetBufferPointer(),
				&pDXVertexShader->m_pVertexShader);
			pCode->Release();
			pCode = NULL;

			{
				D3DXCONSTANTTABLE_DESC ConstantTableDesc;
				hr = ConstantTable->GetDesc(&ConstantTableDesc);
				if (hr != D3D_OK) {
					return false;
				}
				for (unsigned int i = 0; i < ConstantTableDesc.Constants; ++i) {
					D3DXHANDLE ConstantHandle = ConstantTable->GetConstant(NULL, i);
					D3DXCONSTANT_DESC ConstantDesc;
					UINT NumConstants = 1;
					hr = ConstantTable->GetConstantDesc(ConstantHandle, &ConstantDesc,
						&NumConstants);
					if (hr != D3D_OK) {
						return false;
					}
					// Copy the constant and its name into a self-contained data structure, and add it to the constant array.
					RConstant Constant;
					Constant.Name = ConstantDesc.Name;
					Constant.RegisterCount = ConstantDesc.RegisterCount;
					Constant.RegisterIndex = ConstantDesc.RegisterIndex;
					pDXVertexShader->Constants.AddItem(Constant);
				}
			}

			MultiByteToWideChar(CP_ACP, 0, Material->PixelShaderFileNames(i*RShaderPass::ShaderPasses.Size() + j).GetBuffer(), 1024, Chr, 1024);
			wsprintf(FN, TEXT("%s"), Chr);	

			LPD3DXCONSTANTTABLE PixelConstantTable;
			hr = D3DXCompileShaderFromFile(FN, NULL, NULL, "PS", "ps_2_0",
				dwShaderFlags, &pCode, &pErr, &PixelConstantTable);
			if (hr != D3D_OK) {
				char Err[1024];
				sprintf_s(Err, 1024, "%s", (char*) pErr->GetBufferPointer());
				pErr->Release();
				return false;
			}
			GetDevice()->CreatePixelShader((DWORD*) pCode->GetBufferPointer(),
				&pDXPixelShader->m_pPixelShader);
			pCode->Release();

			{
				D3DXCONSTANTTABLE_DESC ConstantTableDesc;
				hr = PixelConstantTable->GetDesc(&ConstantTableDesc);
				if (hr != D3D_OK) {
					return false;
				}
				for (unsigned int i = 0; i < ConstantTableDesc.Constants; ++i) {
					D3DXHANDLE ConstantHandle = PixelConstantTable->GetConstant(NULL,
						i);
					D3DXCONSTANT_DESC ConstantDesc;
					UINT NumConstants = 1;
					hr = PixelConstantTable->GetConstantDesc(ConstantHandle,
						&ConstantDesc, &NumConstants);
					if (hr != D3D_OK) {
						return false;
					}
					// Copy the constant and its name into a self-contained data structure, and add it to the constant array.
					RConstant Constant;
					Constant.Name = ConstantDesc.Name;
					Constant.RegisterCount = ConstantDesc.RegisterCount;
					Constant.RegisterIndex = ConstantDesc.RegisterIndex;
					pDXPixelShader->Constants.AddItem(Constant);
				}
			}
		}
	}	
    return true;
}

bool CDirectXDriver::AssembleShaderFromFile(RShaderBase *pShader) {
    return true;
}

bool CDirectXDriver::CompileShaderFromMemory(RShaderBase *pShader) {
    return true;
}

bool CDirectXDriver::AssembleShaderFromMemory(RShaderBase *pShader) {
    return true;
}

bool CDirectXDriver::SetRenderTarget(unsigned int Idx, RRenderTarget* RT) {
    RDXRenderTarget *DXRT = dynamic_cast<RDXRenderTarget*>(RT);
    if (!DXRT)
        return false;
    HRESULT hr = GetDevice()->SetRenderTarget(Idx, DXRT->m_pRTSurface);
	if(hr != D3D_OK) {
		return false;
	}
    return true;
}

bool CDirectXDriver::SetDepthStencilSurface(RRenderTarget* RT) {
    RDXRenderTarget *DXRT = dynamic_cast<RDXRenderTarget*>(RT);
    if (!DXRT)
        return false;
    GetDevice()->SetDepthStencilSurface(DXRT->m_pRTSurface);
    return true;
}

RRenderTarget* CDirectXDriver::CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat, ETextureUsage TexUsage) {
    RDXRenderTarget *DXRT = new RDXRenderTarget();
    RDXTextureBuffer *DXTex = dynamic_cast<RDXTextureBuffer*>(DXRT->m_pTexture);
    HRESULT hr = D3DXCreateTexture(GetDevice(), Width, Height, 1, GTextureUsage[TexUsage],
                      GPixelFormats[PixelFormat], D3DPOOL_DEFAULT, &DXTex->m_pTexture);
    if (D3D_OK != (DXTex->m_pTexture->GetSurfaceLevel(0, &DXRT->m_pRTSurface)))
        return NULL;
    DXRT->m_SizeX = Width;
    DXRT->m_SizeY = Height;
    return DXRT;
}

RTextureBuffer* CDirectXDriver::CreateTextureBuffer(unsigned int Width, unsigned int Height) {
    RDXTextureBuffer* DXTex = new RDXTextureBuffer();
	D3DXCreateTexture(GetDevice(), Width, Height, 1,
                      GTextureUsage[TexUsage_Dynamic],
                      GPixelFormats[PixelFormat_A8R8G8B8], D3DPOOL_DEFAULT,
                      &DXTex->m_pTexture);
    DXTex->Width = Width;
    DXTex->Height = Height;
    return DXTex;
}

RTextureBuffer* CDirectXDriver::CreateFontBuffer(unsigned int Width, unsigned int Height) {
    RDXFontBuffer* DXTex = new RDXFontBuffer();
    D3DXCreateTexture(GetDevice(), Width, Height, 1,
                      GTextureUsage[TexUsage_Dynamic],
                      GPixelFormats[PixelFormat_A8R8G8B8], D3DPOOL_DEFAULT,
                      &DXTex->m_pTexture);
    DXTex->Width = Width;
    DXTex->Height = Height;
    return DXTex;
}

bool CDirectXDriver::SetStreamSource(RDynamicPrimitiveBuffer* PrimitiveBuffer) {
    RDXVideoMemoryVertexBuffer *DXVertexBuffer =
        dynamic_cast<RDXVideoMemoryVertexBuffer*>(PrimitiveBuffer->m_pVB);
    if (!DXVertexBuffer)
        return false;
    GetDevice()->SetStreamSource(0, DXVertexBuffer->VB, 0,
                                 DXVertexBuffer->Declaration->GetStride());
    return true;
}

bool CDirectXDriver::SetIndices(RDynamicPrimitiveBuffer* PrimitiveBuffer) {
    RDXVideoMemoryIndexBuffer *DXIndexBuffer =
        dynamic_cast<RDXVideoMemoryIndexBuffer*>(PrimitiveBuffer->m_pIB);
    if (!DXIndexBuffer)
        return false;
    GetDevice()->SetIndices(DXIndexBuffer->IB);
    return true;
}

bool CDirectXDriver::SetVertexDeclaration(RVertexDeclaration* Decl) {
	//if(Decl != CurrentVertexDecl) {
		RDXVertexDeclaration* DXDecl = dynamic_cast<RDXVertexDeclaration*>(Decl);
		if(!DXDecl->DXDecl) {
			GetDevice()->CreateVertexDeclaration(DXDecl->DXDeclElements, &DXDecl->DXDecl);
		}
		CurrentVertexDecl = DXDecl;
		GetDevice()->SetVertexDeclaration(DXDecl->DXDecl);
	//}	
    return true;
}

RRenderTarget* CDirectXDriver::GetBackBuffer() {
	return BackBuffer;
}

bool CDirectXDriver::SetViewport(BViewport* Viewport) {
    D3DVIEWPORT9 vp;
    vp.X = Viewport->X;
    vp.Y = Viewport->Y;
    vp.Width = Viewport->Width;
    vp.Height = Viewport->Height;
    vp.MinZ = 0.0f;
    vp.MaxZ = 0.1f;
    return GetDevice()->SetViewport(&vp) == D3D_OK;
}

bool CDirectXDriver::SetClipRect(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height) {
    RECT rt;
    rt.left = x;
    rt.right = x + Width;
    rt.top = y;
    rt.bottom = y + Height;
    return GetDevice()->SetScissorRect(&rt) == D3D_OK;
}

void CDirectXDriver::InitializeVertexDecl(RVertexDeclaration* Decl) {
	RDXVertexDeclaration* DXDecl = dynamic_cast<RDXVertexDeclaration*>(Decl);
	DXDecl->DXDeclElements = new D3DVERTEXELEMENT9[DXDecl->Types.Size()+1];
	WORD Offset = 0;
	unsigned int* UsageIndex = new unsigned int[DeclUsage_End];
	for(unsigned int i=0;i<DeclUsage_End;++i) {
		UsageIndex[i] = 0;
	}
	for(unsigned int i=0;i<DXDecl->Types.Size();++i) {		
		DXDecl->DXDeclElements[i].Stream = 0;
		DXDecl->DXDeclElements[i].Offset = Offset;
		DXDecl->DXDeclElements[i].Type = GVertexDeclType[DXDecl->Types(i)];
		DXDecl->DXDeclElements[i].Method = D3DDECLMETHOD_DEFAULT;
		DXDecl->DXDeclElements[i].Usage = GVertexDeclUsage[DXDecl->Usages(i)];
		DXDecl->DXDeclElements[i].UsageIndex = UsageIndex[GVertexDeclUsage[DXDecl->Usages(i)]]++;
		Offset += DXDecl->GetTypeSize(DXDecl->Types(i));
	}
	delete[] UsageIndex;
	D3DVERTEXELEMENT9 End = D3DDECL_END();
	memcpy(&DXDecl->DXDeclElements[DXDecl->Types.Size()], &End, sizeof(D3DVERTEXELEMENT9));	
}

void CDirectXDriver::SetFillMode(EFillMode FM) {
    if (CurrentFillMode != FM) {
        GetDevice()->SetRenderState(D3DRS_FILLMODE, GFillMode[CurrentFillMode =
                                        FM]);
    }
}

void CDirectXDriver::SetStencilState(TStencilState& StencilState) {
    DX9_SET_STENCILSTATE(D3DRS_STENCILENABLE, EnableStencil);
    if (CurrentStencilState.EnableStencil) {
        bool DoesUseComp = false;
        DX9_SET_STENCILSTATE(D3DRS_STENCILFUNC, StencilTest);
        if (CurrentStencilState.StencilTest != CompareFunc_Always
                || CurrentStencilState.StencilTest != CompareFunc_Never) {
            DX9_SET_STENCILSTATE(D3DRS_STENCILFAIL, StencilFailOp);
            DX9_SET_STENCILSTATE(D3DRS_STENCILZFAIL, StencilZFailOp);
            DX9_SET_STENCILSTATE(D3DRS_STENCILPASS, StencilZPassOp);
            DoesUseComp = true;
        }
        DX9_SET_STENCILSTATE(D3DRS_CCW_STENCILFUNC, EnableTwoSideStencil);
        if (CurrentStencilState.EnableTwoSideStencil) {
            DX9_SET_STENCILSTATE(D3DRS_CCW_STENCILFUNC, BackFaceStencilTest);
            if (CurrentStencilState.BackFaceStencilTest != CompareFunc_Always
                    || CurrentStencilState.BackFaceStencilTest
                    != CompareFunc_Never) {
                DX9_SET_STENCILSTATE(D3DRS_STENCILFAIL, BackFaceStencilFailOp);
                DX9_SET_STENCILSTATE(D3DRS_STENCILZFAIL, BackFaceStencilZFailOp);
                DX9_SET_STENCILSTATE(D3DRS_STENCILPASS, BackFaceStencilZPassOp);
                DoesUseComp = true;
            }
        }
        if (DoesUseComp) {
            DX9_SET_STENCILSTATE(D3DRS_STENCILMASK, StencilReadMask);
            DX9_SET_STENCILSTATE(D3DRS_STENCILWRITEMASK, StencilWriteMask);
            DX9_SET_STENCILSTATE(D3DRS_STENCILREF, StencilRef);
        }
    }
}

void CDirectXDriver::SetBlendState(TBlendState& BlendState) {
    DX9_SET_BLENDSTATE(D3DRS_ALPHABLENDENABLE, EnableAlphaBlend);
    if (CurrentBlendState.EnableAlphaBlend) {
        DX9_SET_BLENDSTATE(D3DRS_BLENDOP, ColorBlendOp);
        DX9_SET_BLENDSTATE(D3DRS_SRCBLEND, SrcColorBlendState);
        DX9_SET_BLENDSTATE(D3DRS_DESTBLEND, DestColorBlendState);
        DX9_SET_BLENDSTATE(D3DRS_BLENDOPALPHA, AlphaBlendOp);
        DX9_SET_BLENDSTATE(D3DRS_SRCBLENDALPHA, SrcAlphaBlendState);
        DX9_SET_BLENDSTATE(D3DRS_DESTBLENDALPHA, DestAlphaBlendState);
    }
}

void CDirectXDriver::SetDepthState(TDepthState& DepthState) {
    if (CurrentDepthState.EnableDepthWrite != DepthState.EnableDepthWrite) {
        GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,
                                    CurrentDepthState.EnableDepthWrite =
                                        DepthState.EnableDepthWrite);
    }
    if (CurrentDepthState.EnableDepthWrite) {
        if (CurrentDepthState.DepthTest != DepthState.DepthTest) {
            GetDevice()->SetRenderState(D3DRS_ZFUNC,
                                        CurrentDepthState.DepthTest = DepthState.DepthTest);
        }
    }
}

void CDirectXDriver::SetCullMode(ECullMode& CullMode) {
    if (CurrentCullMode != CullMode) {
        GetDevice()->SetRenderState(D3DRS_CULLMODE, CullMode);
    }
}