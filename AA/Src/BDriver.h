#pragma once

#include "RResource.h"

class BViewport;
class BPrimitive;
class TBatch;

enum ETextureUsage
{
	TexUsage_RenderTarget,
	TexUsage_DepthStencil,
};

enum EPixelFormat
{
	PF_A8R8G8B8,
	PF_D24S8,
};

enum EFillMode
{
	FillMode_Point,
	FillMode_Wireframe,
	FillMode_Solid
};

class BDriver
{
public:
	BDriver();
	virtual ~BDriver();

	virtual bool CreateDriver() = 0;
	virtual bool DestroyDriver() = 0;

	virtual bool SetStreamSource(RDynamicPrimitiveBuffer* PrimitiveBuffer) = 0;
	virtual bool SetIndices(RDynamicPrimitiveBuffer* PrimitiveBuffer) = 0;

	virtual bool DrawPrimitiveUP(unsigned int NumVertices, unsigned int PrimCount, void* pIndices, unsigned int IndexStride, void* pVertices, unsigned int VertexStride) = 0;
	virtual bool DrawPrimitive(unsigned int NumVertices, unsigned int PrimCount) = 0;

	virtual bool SetTexture(int nStage, RTextureBuffer* pTexture) = 0;

	virtual RDynamicPrimitiveBuffer* CreatePrimitiveBuffer(TBatch* pBatch) = 0;
	virtual RTextureBuffer* CreateTextureBuffer() = 0;

	virtual bool BeginScene() = 0;
	virtual bool EndScene() = 0;

	virtual bool Clear(bool bClearColor = true, unsigned long Color = 0x00000000, bool bClearDepth = false, float Depth = 0.0f, bool bClearStencil = false, unsigned long Stencil = 0.0f) = 0;

	virtual bool CompileShaderFromFile(RShaderBase *pShader) = 0;
	virtual bool AssembleShaderFromFile(RShaderBase *pShader) = 0;
	virtual bool CompileShaderFromMemory(RShaderBase *pShader) = 0;
	virtual bool AssembleShaderFromMemory(RShaderBase *pShader) = 0;

	virtual RRenderTarget* CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat, ETextureUsage TexUsage = TexUsage_RenderTarget) = 0;
	virtual bool SetRenderTarget(unsigned int Idx, RRenderTarget* RT) = 0;
	virtual bool SetDepthStencilSurface(RRenderTarget* RT) = 0;

	virtual RRenderTarget* GetBackBuffer() = 0;
	virtual bool SetViewport(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height, float MinZ, float MaxZ) = 0;
	virtual bool SetClipRect(unsigned int x, unsigned int y, unsigned int Width, unsigned int Height) = 0;

	////////////////////////////////////////////////////// RenderState //////////////////////////////////////////////////
	virtual bool SetFillMode(EFillMode FM) = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

extern BDriver	*GDriver;