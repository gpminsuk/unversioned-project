#pragma once

#include "TDataTypes.h"

class BViewport;

class RShaderBase
{
public:
	RShaderBase() : m_nPass(1) {}

	virtual bool CompileShaderFromFile() = 0;
	virtual bool AssembleShaderFromFile() = 0;
	virtual bool CompileShaderFromMemory() = 0;
	virtual bool AssembleShaderFromMemory() = 0;

	virtual bool BeginShader() = 0;
	virtual bool SetParameter(BViewport* vp) = 0;
	virtual bool EndShader() = 0;
	virtual bool BeginPass() = 0;
	virtual bool EndPass() = 0;

	char m_FileName[256];
	unsigned int m_nPass;
protected:
};

class RShaderTable
{
public:
	static RShaderBase* pShaders;
	static int nTableSize;
};

class RIndexBuffer
{
public:
	RIndexBuffer() : pIndices(0), nIndices(0) {}
	virtual ~RIndexBuffer() { delete[] pIndices; pIndices = 0; nIndices = 0; }

	TIndex16 *pIndices;
	int nIndices;
};

class RIndexBufferTable
{
public:
	static RIndexBuffer* pIndexBuffer;
	static int nTableSize;
};

// primitive per one DP

enum VD_TYPE
{
	DECLTYPE_FLOAT1 = 0,
	DECLTYPE_FLOAT2 = 1,
	DECLTYPE_FLOAT3 = 2,
	DECLTYPE_FLOAT4 = 3,
	DECLTYPE_COLOR = 4,
	DECLTYPE_UBYTE4 = 5,  
	DECLTYPE_SHORT2 = 6,
	DECLTYPE_SHORT4 = 7,
	DECLTYPE_UBYTE4N = 8,
	DECLTYPE_SHORT2N = 9,
	DECLTYPE_SHORT4N = 10,
	DECLTYPE_USHORT2N = 11,
	DECLTYPE_USHORT4N = 12,
	DECLTYPE_UDEC3 = 13,
	DECLTYPE_DEC3N = 14,
	DECLTYPE_FLOAT16_2 = 15,
	DECLTYPE_FLOAT16_4 = 16,
	DECLTYPE_UNUSED = 17,
};

struct VertexDeclaration
{
	short Offset;
	char Type;
};

class RVertexBuffer //: public AMemory
{
public:
	RVertexBuffer() : pVertices(0), nVertices(0) {}
	virtual ~RVertexBuffer() { delete[] pVertices; pVertices = 0; nVertices = 0; }

	int nVertices;
	int nVertexStride;


	VertexDeclaration* Declaration;

	char *pVertices;
};

class RVertexBufferTable
{
public:
	static RVertexBuffer* pVertexBuffer;
	static int nTableSize;
};

class RTexture
{
protected:
	enum ETextureMemoryType
	{
		TMT_SYSTEM,
		TMT_MANAGED,
		TMT_VIDEO
	};
	int Width;
	int Height;
	ETextureMemoryType MemoryType;
};

class RTextureTable
{
public:
	static RTexture* pTextures;
	static int nTableSize;
};

class RMaterial
{
public:
	int TID;
};