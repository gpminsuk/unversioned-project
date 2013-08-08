#pragma once

// Shader
// IndexBuffer
// VertexBuffer
// Texture
#include "TDataTypes.h"

class BViewport;
class BRenderingBatch;
class BLightComponent;

class RVertexDeclaration {
public:
	static RVertexDeclaration* Position_Normal_TexCoord;
	struct Position_Normal_TexCoord_VD {
		TVector3 Position;
		TVector3 Normal;
		TVector2 TexCoord;
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

struct RConstant
{
	TString Name;
	unsigned int RegisterCount;
	unsigned int RegisterIndex;
};

class RShaderPass
{
public:
	static TArray<RShaderPass*> ShaderPasses;

	virtual TString GetName() = 0;
	virtual bool ShouldCompile(class RVertexProtocol* Protocol) = 0;
	virtual void ConfigureLight(class RShader* InPixelShader, RShader* InVertexShader, BRenderingBatch* InBatch) = 0;
};

class RVertexProtocol
{
public:
	static TArray<RVertexProtocol*> Protocols;
	RVertexProtocol() :
		Decl(0) {}

	virtual ~RVertexProtocol() {
	}
	
	class RVertexDeclaration* Decl;

	virtual TString GetName() = 0;
};

class RShader
{
public:
	RShader() :
			ShaderPass(0) {
	}
	~RShader() {
		delete ShaderPass;
	}
	RShaderPass* ShaderPass;
	RVertexDeclaration* VertexProtocol;
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
	RMaterial(TString name);

	int TID;
	TArray<TString> VertexShaderFileNames;
	TArray<TString> PixelShaderFileNames;
	TArray<RShaderBase*> Shaders;

	RShaderBase* FindShader(RVertexProtocol* VertexProtocol, RShaderPass* ShaderPass);
private:
	void ReplaceInclude(FILE* fp, FILE* cachefp, TString VertexProtocolName, TString MaterialName);
};

class RMaterialTable
{
public:
	static TArray<RMaterial*> Materials;
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