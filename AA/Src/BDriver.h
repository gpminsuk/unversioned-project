#pragma once

#include "RResource.h"

class BViewport;
class TPrimitiveTemplateBase;
class TBatch;

enum EPixelFormat
{
	PF_A8R8G8B8,
};

class BTextureBuffer
{
public:
	virtual bool DestroyTextureBuffer() = 0;
};

class BPrimitiveBuffer
{
public:
	virtual bool DestroyVertexBuffer() = 0;
};

class BDriver
{
public:
	virtual bool CreateDriver() = 0;
	virtual bool DestroyDriver() = 0;

	virtual bool DrawPrimitive() = 0;

	virtual bool SetTexture(int nStage, BTextureBuffer* pTexture) = 0;

	virtual BPrimitiveBuffer* CreatePrimitiveBuffer(TBatch* pBatch) = 0;
	virtual BTextureBuffer* CreateTextureBuffer() = 0;

	virtual bool BeginScene() = 0;
	virtual bool EndScene() = 0;

	virtual bool CompileShaderFromFile(RShaderBase *pShader) = 0;
	virtual bool AssembleShaderFromFile(RShaderBase *pShader) = 0;
	virtual bool CompileShaderFromMemory(RShaderBase *pShader) = 0;
	virtual bool AssembleShaderFromMemory(RShaderBase *pShader) = 0;

	virtual RRenderTarget* CreateRenderTarget(unsigned int Width, unsigned int Height, EPixelFormat PixelFormat) = 0;
	virtual bool SetRenderTarget(unsigned int Idx, RRenderTarget* RT) = 0;

	virtual RRenderTarget* GetBackBuffer() = 0;
};

extern BDriver	*GDriver;