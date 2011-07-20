#pragma once

// Shader
// IndexBuffer
// VertexBuffer
// Texture
#include "TDataTypes.h"

class BViewport;
class BRenderingBatch;
class BLightComponent;

struct RConstant
{
	TString Name;
	unsigned int RegisterCount;
	unsigned int RegisterIndex;
};

class RVertexBuilder
{

};

class RShaderConfigure
{
public:
	RShaderConfigure(class RShader* InShader) : Shader(InShader) {}

	class RShader* Shader;

	virtual void ConfigureLight(BRenderingBatch* InBatch) = 0;
};

class RShader
{
public:
	RShader() : Configure(0) {}
	~RShader() { delete Configure; }
	RShaderConfigure* Configure;
	TArray<RConstant> Constants;

	virtual bool SetShaderConstantF(TString VarName, float* Value) = 0;
};

class RPixelShader : public RShader
{
public:
	virtual bool SetShaderConstantF(TString VarName, float* Value) = 0;
};

class RVertexShader : public RShader
{
public:
	virtual bool SetShaderConstantF(TString VarName, float* Value) = 0;
};

class RShaderBase
{
public:
	RPixelShader* PixelShader;
	RVertexShader* VertexShader;

	RShaderBase() :
		VertexShader(0),
		PixelShader(0)
	{

	}
	virtual ~RShaderBase()
	{
		delete VertexShader;
		delete PixelShader;
	}

	virtual bool BeginShader() = 0;
	virtual bool SetParameter(BViewport* vp) = 0;
	virtual bool EndShader() = 0;

	wchar_t m_FileName[256];
protected:
};

class RShaderTable
{
public:
	static TArray<RShaderBase*> Shaders;
};

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

class RVideoMemoryIndexBuffer
{
public:
	RVideoMemoryIndexBuffer() : nIndices(0) {}
	virtual ~RVideoMemoryIndexBuffer()
	{
		nIndices = 0;
	}

	unsigned int nIndices;
};

class RVideoMemoryVertexBuffer
{
public:
	RVideoMemoryVertexBuffer() : nVertices(0), Declaration(0) {}
	virtual ~RVideoMemoryVertexBuffer()
	{
		delete[] Declaration; Declaration = 0;
		nVertices = 0;
	}

	unsigned int nVertices;
	unsigned int nVertexStride;

	VertexDeclaration* Declaration;
};

class RPrimitiveBuffer
{
public:
};

class RMaterial
{
public:
	int TID;
};

class RRenderTarget
{
public:
	RRenderTarget() : m_pTexture(0) {}
	virtual ~RRenderTarget() {}

	class RTextureBuffer* m_pTexture;

	unsigned int m_SizeX;
	unsigned int m_SizeY;

	virtual bool Release() = 0;
};