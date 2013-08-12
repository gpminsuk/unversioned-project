#pragma once

#include "BDriver.h"
#include <d3d9.h>
#include <d3dx9effect.h>
#include <windows.h>

class RDXRenderTarget;

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

class CDirectXDriver: public BDriver
{
public:
	CDirectXDriver();
	virtual ~CDirectXDriver();
	inline LPDIRECT3DDEVICE9 GetDevice() {
		return m_pDevice;
	}

	virtual bool CreateDriver(AApplication* Application);
	virtual bool ResizeBackBuffer(int Width, int Height);
	virtual bool DestroyDriver();

	virtual bool SetIndices(RDynamicPrimitiveBuffer* PrimitiveBuffer);
	virtual bool SetStreamSource(RDynamicPrimitiveBuffer* PrimitiveBuffer);
	virtual bool SetVertexDeclaration(RVertexDeclaration* Decl);

	virtual bool DrawPrimitive(EPrimitiveType PrimitiveType, UINT PrimCount);
	virtual bool DrawIndexedPrimitive(EPrimitiveType PrimitiveType, UINT NumVertices, UINT PrimCount);
	virtual bool DrawPrimitiveUP(EPrimitiveType PrimitiveType, unsigned int NumVertices, void* pVertices, unsigned int VertexStride);
	virtual bool DrawIndexedPrimitiveUP(EPrimitiveType PrimitiveType, UINT NumVertices, UINT PrimCount, PVOID pIndices, UINT IndexStride, PVOID pVertices, UINT VertexStride);

	virtual bool SetTexture(int nStage, RTextureBuffer* pTexture);

	virtual RDynamicPrimitiveBuffer* CreatePrimitiveBuffer(BRenderingBatch* pBatch);
	virtual RTextureBuffer* CreateTextureBuffer(const TString& Str);

	virtual bool BeginScene(BViewport* Viewport);
	virtual bool EndScene(BViewport* Viewport);

	virtual bool Clear(bool bClearColor = true, DWORD Color = 0x00000000, bool bClearDepth =
			false, float Depth = 0.0f, bool bClearStencil = false, DWORD Stencil =
			0.0f);

	virtual bool CompileMaterial(RMaterial* Material);
	virtual bool AssembleShaderFromFile(RShaderBase *pShader);
	virtual bool CompileShaderFromMemory(RShaderBase *pShader);
	virtual bool AssembleShaderFromMemory(RShaderBase *pShader);

	virtual RRenderTarget* CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat, ETextureUsage TexUsage =
			TexUsage_RenderTarget);
	virtual RTextureBuffer* CreateTextureBuffer(unsigned int Width, unsigned int Height);
	virtual RTextureBuffer* CreateFontBuffer(unsigned int Width, unsigned int Height);
	virtual bool SetRenderTarget(unsigned int Idx, RRenderTarget* RT);
	virtual bool SetDepthStencilSurface(RRenderTarget* RT);

	virtual RRenderTarget* GetBackBuffer();
	virtual bool SetViewport(BViewport* Viewport);
	virtual bool SetClipRect(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height);

	virtual void InitializeVertexDecl(RVertexDeclaration* Decl);

	////////////////////////////////////////////////////// RenderState //////////////////////////////////////////////////
	virtual void SetFillMode(EFillMode FM);
	virtual void SetCullMode(ECullMode& CullMode);

	virtual void SetStencilState(TStencilState& StencilState);
	virtual void SetDepthState(TDepthState& DepthState);
	virtual void SetBlendState(TBlendState& BlendState);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3D9 m_pD3D;

	RRenderTarget* BackBuffer;
};
