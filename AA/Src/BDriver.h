#pragma once

class BViewport;
class TPrimitiveTemplateBase;
class TBatch;

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
};

extern BDriver	*GDriver;