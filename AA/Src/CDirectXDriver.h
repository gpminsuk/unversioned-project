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

	virtual bool SetIndices(BPrimitiveBuffer* PrimitiveBuffer);
	virtual bool SetStreamSource(BPrimitiveBuffer* PrimitiveBuffer);

	virtual bool DrawPrimitive(UINT NumVertices, UINT PrimCount);
	virtual bool DrawPrimitiveUP(UINT NumVertices, UINT PrimCount, PVOID pIndices, UINT IndexStride, PVOID pVertices, UINT VertexStride);

	virtual bool SetTexture(int nStage, RTextureBuffer* pTexture);

	virtual BPrimitiveBuffer* CreatePrimitiveBuffer(TBatch* pBatch);
	virtual RTextureBuffer* CreateTextureBuffer();

	virtual bool BeginScene();
	virtual bool EndScene();

	virtual bool Clear(bool bClearColor = true, DWORD Color = 0x00000000, bool bClearDepth = false, float Depth = 0.0f, bool bClearStencil = false, DWORD Stencil = 0.0f);

	virtual bool CompileShaderFromFile(RShaderBase *pShader);
	virtual bool AssembleShaderFromFile(RShaderBase *pShader);
	virtual bool CompileShaderFromMemory(RShaderBase *pShader);
	virtual bool AssembleShaderFromMemory(RShaderBase *pShader);

	virtual RRenderTarget* CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat);
	virtual bool SetRenderTarget(unsigned int Idx, RRenderTarget* RT);

	virtual RRenderTarget* GetBackBuffer();

	class RDXRenderTarget* BackBuffer;

	LPDIRECT3DDEVICE9	m_pDevice;
private:
	TWindowInfo*		m_pWindow;

	LPDIRECT3D9			m_pD3D;
};

class CDirectXPrimitiveBuffer : public BPrimitiveBuffer
{
public:
	virtual bool DestroyVertexBuffer();

	unsigned int VertexStride;
	
	IDirect3DVertexBuffer9* VB;
	IDirect3DIndexBuffer9* IB;
};