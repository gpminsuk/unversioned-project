#pragma once

#include "RResource.h"

class BPrimitive;

class RBaseShaderPass: public RShaderPass
{
public:
	virtual void ConfigureLight(RShader* InPixelShader, RShader* InVertexShader, BRenderingBatch* InBatch);

	virtual TString GetName() {
		return "Base";
	}
};

class RRenderTargetShaderPass: public RShaderPass
{
public:
	virtual void ConfigureLight(RShader* InPixelShader, RShader* InVertexShader, BRenderingBatch* InBatch);

	virtual TString GetName() {
		return "RenderTarget";
	}
};

class RStaticMeshVertexProtocol : public RVertexProtocol {
public:
	RStaticMeshVertexProtocol() {
		Decl = RVertexDeclaration::Position_Normal_TexCoord;
	}

	virtual TString GetName() {
		return "StaticMesh";
	}
};

class RRenderTargetVertexProtocol : public RVertexProtocol {
public:
	RRenderTargetVertexProtocol() {
		Decl = RVertexDeclaration::Position_TexCoord;
	}

	virtual TString GetName() {
		return "RenderTarget";
	}
};