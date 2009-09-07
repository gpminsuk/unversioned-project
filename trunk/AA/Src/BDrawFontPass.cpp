#include "StdAfx.h"

#include "RResource.h"

#include "BDrawFontPass.h"
#include "BDriver.h"
#include "BViewport.h"
#include "BRenderer.h"

#include "UFreeTypeDrawer.h"

BDrawFontPass::BDrawFontPass()
{
}

BDrawFontPass::~BDrawFontPass()
{
}

void BDrawFontPass::BeginPass(BViewport* Viewport)
{
	m_Viewport = Viewport;

	RShaderBase* pShader = RShaderTable::Shaders(3);
	GDriver->SetRenderTarget(0, GDriver->GetBackBuffer());
	pShader->BeginShader();
	pShader->SetParameter(Viewport);
}

void BDrawFontPass::EndPass()
{
	RShaderBase* pShader = RShaderTable::Shaders(3);
	pShader->EndShader();
}

void BDrawFontPass::DrawPrimitive(float fps)
{
	TString Str;
	sprintf_s(Str.Str, 1024, "FPS = %.3f", fps);
	GFontDrawer->DrawString(Str, RTextureBufferTable::TextureBuffers(1));

	GDriver->SetTexture(0, RTextureBufferTable::TextureBuffers(1));

	struct VD
	{
		TVector3 Pos;
		TVector2 UV;
	};

	VD Vertices[4];

	float OffsetX = 0.5f/m_Viewport->m_Width;
	float OffsetY = 0.5f/m_Viewport->m_Height;

	TVector2 TopLeft(0,0);
	TVector2 BottomRight;
	BottomRight.x = TopLeft.x + (float)RTextureBufferTable::TextureBuffers(1)->Width/(float)m_Viewport->m_Width;
	BottomRight.y = TopLeft.y + (float)RTextureBufferTable::TextureBuffers(1)->Height/(float)m_Viewport->m_Height;

	TopLeft *= 2.0f;
	TopLeft -= 1.0f;
	BottomRight *= 2.0f;
	BottomRight -= 1.0f;

	Vertices[0].Pos = TVector3(TopLeft.x - OffsetX, TopLeft.y - OffsetY, 0.0f);
	Vertices[1].Pos = TVector3(TopLeft.x - OffsetX, BottomRight.y - OffsetY, 0.0f);
	Vertices[2].Pos = TVector3(BottomRight.x - OffsetX, TopLeft.y - OffsetY, 0.0f);
	Vertices[3].Pos = TVector3(BottomRight.x - OffsetX, BottomRight.y - OffsetY, 0.0f);

	Vertices[0].UV = TVector2(0.0f, 1.0f);
	Vertices[1].UV = TVector2(0.0f, 0.0f);
	Vertices[2].UV = TVector2(1.0f, 1.0f);
	Vertices[3].UV = TVector2(1.0f, 0.0f);

	TIndex16 Indices[2];
	Indices[0] = TIndex16(0, 1, 2);
	Indices[1] = TIndex16(1, 3, 2);

	GDriver->SetVertexDeclaration(VertexType_Position | VertexType_UV);
	GDriver->DrawIndexedPrimitiveUP(PrimitiveType_TriangleList, 4, 2, Indices, sizeof(TIndex16)/3, Vertices, sizeof(VD));
}