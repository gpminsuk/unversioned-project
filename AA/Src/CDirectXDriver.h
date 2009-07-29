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
	inline LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }

	bool CompileShader();

	virtual bool CreateDriver();
	virtual bool DestroyDriver();
	virtual bool DrawPrimitive();

	virtual bool SetTexture(int nStage, BTextureBuffer* pTexture);

	virtual BPrimitiveBuffer* CreatePrimitiveBuffer(TBatch* pBatch);
	virtual BTextureBuffer* CreateTextureBuffer();

	virtual bool BeginScene();
	virtual bool EndScene();

	virtual bool CompileShaderFromFile(RShaderBase *pShader);
	virtual bool AssembleShaderFromFile(RShaderBase *pShader);
	virtual bool CompileShaderFromMemory(RShaderBase *pShader);
	virtual bool AssembleShaderFromMemory(RShaderBase *pShader);

	virtual RRenderTarget* CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat);
	virtual bool SetRenderTarget(unsigned int Idx, RRenderTarget* RT);

	virtual RRenderTarget* GetBackBuffer();

	LPDIRECT3DDEVICE9	m_pDevice;
private:
	TWindowInfo*		m_pWindow;

	LPDIRECT3D9			m_pD3D;
};

class CDirectXVertexBuffer : public BPrimitiveBuffer
{
public:
	virtual bool DestroyVertexBuffer();
	
	IDirect3DVertexBuffer9* VB;
	IDirect3DIndexBuffer9* IB;
};

class CDirectXTextureBuffer : public BTextureBuffer
{
public:
	virtual bool DestroyTextureBuffer();

	IDirect3DTexture9* pTexture;
};