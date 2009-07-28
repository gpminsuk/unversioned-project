#include "RResource.h"

class RDirectXShader : public RShaderBase
{
public:
	RDirectXShader() {}

	struct IDirect3DVertexShader9* m_pVertexShader;
	struct IDirect3DPixelShader9* m_pPixelShader;

	struct IDirect3DVertexDeclaration9* m_pDecl;

	virtual bool BeginShader();
	virtual bool SetParameter(BViewport* vp);
	virtual bool EndShader();
};
