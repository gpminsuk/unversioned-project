#include "StdAfx.h"
#include "BTextDrawer.h"
#include "UFreeTypeDrawer.h"

#include "BDriver.h"
#include "BViewport.h"
#include "RTexture.h"

BTextDrawer* GTextDrawer;

BTextDrawer::BTextDrawer(void)
{
}

BTextDrawer::~BTextDrawer(void)
{
}

void BTextDrawer::AddText(TString& Str)
{
	TText Text;
	Text.Str = Str;
	Text.TextBuffer = GDriver->CreateFontBuffer(Str.GetLength()*20,30);
	Texts.AddItem(Text);
}

void BTextDrawer::DrawTexts(BViewport* Viewport)
{
	RShaderBase* pShader = RShaderTable::Shaders(1);
	GDriver->SetFillMode(FillMode_Solid);
	GDriver->SetRenderTarget(0, GDriver->GetBackBuffer());
	GDriver->SetVertexDeclaration(VertexType_Position | VertexType_UV);
	GDriver->SetBlendState(TBlendState(BlendOp_Add, BlendState_SrcAlpha, BlendState_InvSrcAlpha));

	for(unsigned int i=0;i<Texts.Size();++i)
	{
		GFontDrawer->DrawString(Texts[i].Str, Texts[i].TextBuffer);

		GDriver->SetTexture(0, Texts[i].TextBuffer);
		pShader->BeginShader();

		struct VD
		{
			TVector3 Pos;
			TVector2 UV;
		};

		VD Vertices[4];

		float OffsetX = 0.5f/Viewport->m_Width;
		float OffsetY = 0.5f/Viewport->m_Height;

		float LineOffset = 1.0f / 10.0f;

		Vertices[0].Pos = TVector3(-0.9f, -0.9f + LineOffset * i, 0.0f);
		Vertices[1].Pos = TVector3(-0.9f, -0.9f + LineOffset * (i + 1), 0.0f);
		Vertices[2].Pos = TVector3(-0.9f + ((float)Texts[i].TextBuffer->Width*LineOffset*20.0f/Viewport->m_Width), -0.9f + LineOffset * i, 0.0f);
		Vertices[3].Pos = TVector3(-0.9f + ((float)Texts[i].TextBuffer->Width*LineOffset*20.0f/Viewport->m_Width), -0.9f + LineOffset * (i + 1), 0.0f);

		Vertices[0].UV = TVector2(0.0f - OffsetX, 1.0f - OffsetY);
		Vertices[1].UV = TVector2(0.0f - OffsetX, 0.0f - OffsetY);
		Vertices[2].UV = TVector2(1.0f - OffsetX, 1.0f - OffsetY);
		Vertices[3].UV = TVector2(1.0f - OffsetX, 0.0f - OffsetY);

		TIndex16 Indices[2];
		Indices[0] = TIndex16(0, 1, 2);
		Indices[1] = TIndex16(1, 3, 2);

		GDriver->DrawIndexedPrimitiveUP(PrimitiveType_TriangleList, 4, 2, Indices, sizeof(TIndex16)/3, Vertices, sizeof(VD));
	}

	Texts.Clear(false);

	GDriver->SetBlendState(TBlendState());
}