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
	
	virtual bool ShouldCompile(RVertexProtocol* Protocol) {
		if(Protocol->GetName() == "RenderTarget") {
			return false;
		}
		return true;
	}
};

class RRenderTargetShaderPass: public RShaderPass
{
public:
	virtual void ConfigureLight(RShader* InPixelShader, RShader* InVertexShader, BRenderingBatch* InBatch);

	virtual TString GetName() {
		return "RenderTarget";
	}

	virtual bool ShouldCompile(RVertexProtocol* Protocol) {
		if(Protocol->GetName() == "RenderTarget") {
			return true;
		}
		return false;
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