#pragma once

#include "RResource.h"
#include <d3d9.h>
#include <d3dx9effect.h>

#pragma comment(lib, "d3dx9.lib")

class BViewport;

class CDirectXShader : public RShaderBase
{
public:

	IDirect3DVertexShader9* m_pVertexShader;
	IDirect3DPixelShader9* m_pPixelShader;
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;

	virtual bool CompileShaderFromFile();
	virtual bool AssembleShaderFromFile();
	virtual bool CompileShaderFromMemory();
	virtual bool AssembleShaderFromMemory();

	virtual bool BeginShader();
	virtual bool SetParameter(BViewport* vp);
	virtual bool EndShader();
};
