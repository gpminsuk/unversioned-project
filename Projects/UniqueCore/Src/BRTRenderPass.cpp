#include "StdAfx.h"

#include "RResource.h"

#include "BRTRenderPass.h"
#include "BDriver.h"

BRTRenderPass::BRTRenderPass()
{
}

BRTRenderPass::~BRTRenderPass()
{	
}

void BRTRenderPass::DrawPrimitive()
{
	struct VD
	{
		TVector3 Pos;
		TVector2 UV;
	};

	VD Vertices[4];

	float OffsetX = 0.5f/RPR->m_BaseSceneRT->m_SizeX;
	float OffsetY = 0.5f/RPR->m_BaseSceneRT->m_SizeY;

	Vertices[0].Pos = TVector3(-1.0f, -1.0f, 0.0f);
	Vertices[1].Pos = TVector3(-1.0f,  1.0f, 0.0f);
	Vertices[2].Pos = TVector3( 1.0f, -1.0f, 0.0f);
	Vertices[3].Pos = TVector3( 1.0f,  1.0f, 0.0f);

	Vertices[0].UV = TVector2(0.0f - OffsetX, 1.0f - OffsetY);
	Vertices[1].UV = TVector2(0.0f - OffsetX, 0.0f - OffsetY);
	Vertices[2].UV = TVector2(1.0f - OffsetX, 1.0f - OffsetY);
	Vertices[3].UV = TVector2(1.0f - OffsetX, 0.0f - OffsetY);

	TIndex16 Indices[2];
	Indices[0] = TIndex16(0, 1, 2);
	Indices[1] = TIndex16(1, 3, 2);

	GDriver->DrawIndexedPrimitiveUP(PrimitiveType_TriangleList, 4, 2, Indices, sizeof(TIndex16)/3, Vertices, sizeof(VD));
}