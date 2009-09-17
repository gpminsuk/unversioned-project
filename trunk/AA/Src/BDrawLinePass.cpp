#include "StdAfx.h"

#include "RResource.h"

#include "AThread.h"

#include "BDrawLinePass.h"

#include "BLineBatcher.h"
#include "BViewport.h"
#include "BDriver.h"

BDrawLinePass::BDrawLinePass()
{
}

BDrawLinePass::~BDrawLinePass()
{
}

void BDrawLinePass::BeginPass(BViewport* Viewport)
{
	m_Viewport = Viewport;
	RShaderBase* pShader = RShaderTable::Shaders(2);
	GDriver->SetRenderTarget(0, GDriver->GetBackBuffer());
	GDriver->Clear(false, 0x00000000,true, 1.0f);

	pShader->BeginShader();
	pShader->SetParameter(Viewport);

	GDriver->SetVertexDeclaration(VertexType_Position);
}

void BDrawLinePass::EndPass()
{
	RShaderBase* pShader = RShaderTable::Shaders(2);
	pShader->EndShader();
}

void BDrawLinePass::DrawPrimitive(BLineBatcher* LineBatcher)
{
	size_t NumLines = LineBatcher->Lines.Size();
	if(NumLines > 0)
	{
		struct VD
		{
			TVector3 Pos;
		};

		VD *Vertices = new VD[(NumLines*2)];
		for(unsigned int i=0;i<NumLines;++i)
		{
			Vertices[i*2+0].Pos = LineBatcher->Lines(i).Point1;
			Vertices[i*2+1].Pos = LineBatcher->Lines(i).Point2;
		}
		GDriver->DrawPrimitiveUP(PrimitiveType_LineList, (unsigned int)(NumLines), Vertices, sizeof(VD));
		delete Vertices;
	}
}