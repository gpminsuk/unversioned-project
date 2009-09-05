#pragma once

#include "TWindowInfo.h"

#include "BDriver.h"
#include <d3d9.h>
#include <d3dx9effect.h>

#pragma comment(lib, "..\\d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

class CDirectXDriver : public BDriver
{
public:
	CDirectXDriver(TWindowInfo *Window);
	virtual ~CDirectXDriver();
	inline LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }

	virtual bool CreateDriver();
	virtual bool DestroyDriver();

	virtual bool SetIndices(RDynamicPrimitiveBuffer* PrimitiveBuffer);
	virtual bool SetStreamSource(RDynamicPrimitiveBuffer* PrimitiveBuffer);
	virtual bool SetVertexDeclaration(unsigned long Type);

	virtual bool DrawPrimitive(EPrimitiveType PrimitiveType, UINT PrimCount);
	virtual bool DrawIndexedPrimitive(EPrimitiveType PrimitiveType, UINT NumVertices, UINT PrimCount);
	virtual bool DrawPrimitiveUP(EPrimitiveType PrimitiveType, unsigned int NumVertices, void* pVertices, unsigned int VertexStride);
	virtual bool DrawIndexedPrimitiveUP(EPrimitiveType PrimitiveType, UINT NumVertices, UINT PrimCount, PVOID pIndices, UINT IndexStride, PVOID pVertices, UINT VertexStride);

	virtual bool SetTexture(int nStage, RTextureBuffer* pTexture);

	virtual RDynamicPrimitiveBuffer* CreatePrimitiveBuffer(TBatch* pBatch);
	virtual RTextureBuffer* CreateTextureBuffer();

	virtual bool BeginScene();
	virtual bool EndScene();

	virtual bool Clear(bool bClearColor = true, DWORD Color = 0x00000000, bool bClearDepth = false, float Depth = 0.0f, bool bClearStencil = false, DWORD Stencil = 0.0f);

	virtual bool CompileShaderFromFile(RShaderBase *pShader);
	virtual bool AssembleShaderFromFile(RShaderBase *pShader);
	virtual bool CompileShaderFromMemory(RShaderBase *pShader);
	virtual bool AssembleShaderFromMemory(RShaderBase *pShader);

	virtual RRenderTarget* CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat, ETextureUsage TexUsage = TexUsage_RenderTarget);
	virtual RTextureBuffer* CreateTextureBuffer(unsigned int Width, unsigned int Height);
	virtual RTextureBuffer* CreateFontBuffer(unsigned int Width, unsigned int Height);
	virtual bool SetRenderTarget(unsigned int Idx, RRenderTarget* RT);
	virtual bool SetDepthStencilSurface(RRenderTarget* RT);

	virtual RRenderTarget* GetBackBuffer();
	virtual bool SetViewport(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height, float MinZ, float MaxZ);
	virtual bool SetClipRect(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height);

	////////////////////////////////////////////////////// RenderState //////////////////////////////////////////////////
	virtual bool SetFillMode(EFillMode FM);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class RDXRenderTarget* BackBuffer;

	LPDIRECT3DDEVICE9	m_pDevice;
	LPDIRECT3D9			m_pD3D;
private:
	TWindowInfo*		m_pWindow;

	IDirect3DVertexDeclaration9* m_Declarations[VertexType_End];
};