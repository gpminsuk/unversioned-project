#pragma once

#include "BDriver.h"
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

class CWindowApp;

class CDirectXDriver : public BDriver
{
public:
	CDirectXDriver(CWindowApp *Window);
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

private:
	CWindowApp*			m_pWindow;

	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pDevice;
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