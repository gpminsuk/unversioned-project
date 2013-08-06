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
	RShaderConfigure(class RShader* InShader) :
			Shader(InShader) {
	}

	class RShader* Shader;

	virtual void ConfigureLight(BRenderingBatch* InBatch) = 0;
};

class RShader
{
public:
	RShader() :
			Configure(0) {
	}
	~RShader() {
		delete Configure;
	}
	RShaderConfigure* Configure;
	TArray<RConstant> Constants;

	virtual bool SetShaderConstantF(TString VarName, float* Value) = 0;
};

class RPixelShader: public RShader
{
public:
	virtual bool SetShaderConstantF(TString VarName, float* Value) = 0;
};

class RVertexShader: public RShader
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

class RVertexDeclaration {
public:
	static RVertexDeclaration* Position_Normal_TexCoord;
	struct Position_Normal_TexCoord_VD {
		TVector3 Position;
		TVector3 Normal;
		TVector2 TexCoord;
	};
	static RVertexDeclaration* Position_Normal;
	struct Position_Normal_VD {
		TVector3 Position;
		TVector3 Normal;
	};
	static RVertexDeclaration* Position_TexCoord;
	struct Position_TexCoord_VD {
		TVector3 Position;
		TVector2 TexCoord;
	};
	static RVertexDeclaration* SkeletalMesh_GPU_Skin;	
	struct SkeletalMesh_GPU_Skin_VD {
		TVector3 Position;
		TVector3 Normal;
		TVector2 TexCoord;
		char BoneIndices[4];
		float BoneWeights[4];
	};

	TArray<unsigned int> Types;
	TArray<unsigned int> Usages;

	virtual unsigned int GetTypeSize(unsigned int Type) = 0;
	virtual unsigned int GetStride() = 0;
};

class RVideoMemoryIndexBuffer
{
public:
	RVideoMemoryIndexBuffer() :
			nIndices(0) {
	}
	virtual ~RVideoMemoryIndexBuffer()
	{
		nIndices = 0;
	}

	unsigned int nIndices;
};

class RVideoMemoryVertexBuffer
{
public:
	RVideoMemoryVertexBuffer() :
			nVertices(0) {
	}
	virtual ~RVideoMemoryVertexBuffer()
	{
		nVertices = 0;
	}

	unsigned int nVertices;

	RVertexDeclaration* Declaration;
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
	RRenderTarget() :
			m_pTexture(0) {
	}
	virtual ~RRenderTarget() {
	}

	class RTextureBuffer* m_pTexture;

	unsigned int m_SizeX;
	unsigned int m_SizeY;

	virtual bool Release() = 0;
};