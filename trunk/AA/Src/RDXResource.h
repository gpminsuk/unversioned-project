#include "RResource.h"

class RDirectXShader : public RShaderBase
{
public:
	RDirectXShader() {}

	struct IDirect3DVertexShader9* m_pVertexShader;
	struct IDirect3DPixelShader9* m_pPixelShader;

	struct IDirect3DVertexDeclaration9* m_pDecl;

	virtual bool BeginShader();
	virtual bool SetParameter(BViewport* vp);
	virtual bool EndShader();
};

class RDXRenderTarget : public RRenderTarget
{
public:
	struct IDirect3DSurface9* m_pRTSurface;

	virtual bool Release();

	RDXRenderTarget();
	virtual ~RDXRenderTarget();
};

class RDXTextureBuffer : public RTextureBuffer
{
public:
	RDXTextureBuffer() : m_pTexture(0) {}
	virtual ~RDXTextureBuffer();

	struct IDirect3DTexture9* m_pTexture;
};

class RDXVideoMemoryVertexBuffer : public RVideoMemoryVertexBuffer
{
public:
	struct IDirect3DVertexBuffer9* VB;
};

class RDXVideoMemoryIndexBuffer : public RVideoMemoryIndexBuffer
{
public:
	struct IDirect3DIndexBuffer9* IB;
};

class RDXDynamicPrimitiveBuffer : public RDynamicPrimitiveBuffer
{
public:
	RDXDynamicPrimitiveBuffer();
	virtual ~RDXDynamicPrimitiveBuffer();

	virtual void Release();
};