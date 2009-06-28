#include "stdafx.h"
#include "TDataTypes.h"
#include "RResource.h"

TPrimitiveTemplateBase::~TPrimitiveTemplateBase()
{
	delete pVertexBuffer; pVertexBuffer = 0;
	delete pIndexBuffer; pIndexBuffer = 0;
	delete pShader; pShader = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void TBox::Create()
{	
	pVertexBuffer = new RVertexBuffer();

	pVertexBuffer->Declaration = new VertexDeclaration[2];
	pVertexBuffer->Declaration[0].Offset = 0;
	pVertexBuffer->Declaration[0].Type = DECLTYPE_FLOAT3;
	pVertexBuffer->Declaration[1].Offset = 12;
	pVertexBuffer->Declaration[1].Type = DECLTYPE_FLOAT2;

	struct VD
	{
		TVector3 Pos;
		TVector2 UV;
	};

	pVertexBuffer->nVertexStride = sizeof(VD);
	pVertexBuffer->nVertices = 8;
	pVertexBuffer->pVertices = new char[pVertexBuffer->nVertexStride*pVertexBuffer->nVertices];	

	VD *Vertex = reinterpret_cast<VD*>(pVertexBuffer->pVertices);

	Vertex[0].Pos = TVector3(Extent/2);
	Vertex[7].Pos  = TVector3(-Extent/2);
	Vertex[1].Pos  = TVector3(Vertex[0].Pos.x, Vertex[0].Pos.y, Vertex[7].Pos.z);
	Vertex[2].Pos  = TVector3(Vertex[0].Pos.x, Vertex[7].Pos.y, Vertex[0].Pos.z);
	Vertex[3].Pos  = TVector3(Vertex[0].Pos.x, Vertex[7].Pos.y, Vertex[7].Pos.z);
	Vertex[4].Pos  = TVector3(Vertex[7].Pos.x, Vertex[0].Pos.y, Vertex[0].Pos.z);
	Vertex[5].Pos  = TVector3(Vertex[7].Pos.x, Vertex[0].Pos.y, Vertex[7].Pos.z);
	Vertex[6].Pos  = TVector3(Vertex[7].Pos.x, Vertex[7].Pos.y, Vertex[0].Pos.z);

	Vertex[0].UV = TVector2(0.0f,0.0f);
	Vertex[1].UV = TVector2(0.0f,1.0f);
	Vertex[2].UV = TVector2(0.0f,0.0f);
	Vertex[3].UV = TVector2(0.0f,1.0f);
	Vertex[4].UV = TVector2(1.0f,0.0f);
	Vertex[5].UV = TVector2(1.0f,1.0f);
	Vertex[6].UV = TVector2(1.0f,0.0f);
	Vertex[7].UV = TVector2(1.0f,1.0f);

	pIndexBuffer = new RIndexBuffer();
	pIndexBuffer->nIndices = 12;
	pIndexBuffer->pIndices = new TIndex16[12];
	pIndexBuffer->pIndices[0] = TIndex16(0,2,1);
	pIndexBuffer->pIndices[1] = TIndex16(1,2,3);
	pIndexBuffer->pIndices[2] = TIndex16(4,6,0);
	pIndexBuffer->pIndices[3] = TIndex16(0,6,2);
	pIndexBuffer->pIndices[4] = TIndex16(5,7,4);
	pIndexBuffer->pIndices[5] = TIndex16(4,7,6);
	pIndexBuffer->pIndices[6] = TIndex16(1,3,5);
	pIndexBuffer->pIndices[7] = TIndex16(5,3,7);
	pIndexBuffer->pIndices[8] = TIndex16(5,4,1);
	pIndexBuffer->pIndices[9] = TIndex16(1,4,0);
	pIndexBuffer->pIndices[10] = TIndex16(2,3,6);
	pIndexBuffer->pIndices[11] = TIndex16(6,3,7);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void TQuad::Create()
{
	pVertexBuffer = new RVertexBuffer();

	pVertexBuffer->Declaration = new VertexDeclaration[2];
	pVertexBuffer->Declaration[0].Offset = 0;
	pVertexBuffer->Declaration[0].Type = DECLTYPE_FLOAT3;
	pVertexBuffer->Declaration[1].Offset = 12;
	pVertexBuffer->Declaration[1].Type = DECLTYPE_FLOAT2;

	struct VD
	{
		TVector3 Pos;
		TVector2 UV;
	};

	pVertexBuffer->nVertexStride = sizeof(VD);
	pVertexBuffer->nVertices = 4;
	pVertexBuffer->pVertices = new char[pVertexBuffer->nVertexStride*pVertexBuffer->nVertices];
	VD *Vertex = reinterpret_cast<VD*>(pVertexBuffer->pVertices);
	Vertex[0].Pos = TVector3(-100.0f, Extent.x * -100.0f + Extent.z * -100.0f - Extent.w, -100.0f);
	Vertex[1].Pos = TVector3(-100.0f, Extent.x * -100.0f + Extent.z *  100.0f - Extent.w,  100.0f);
	Vertex[2].Pos = TVector3( 100.0f, Extent.x *  100.0f + Extent.z *  100.0f - Extent.w,  100.0f);
	Vertex[3].Pos = TVector3( 100.0f, Extent.x *  100.0f + Extent.z * -100.0f - Extent.w, -100.0f);

	Vertex[0].UV = TVector2(0.0f,0.0f);
	Vertex[1].UV = TVector2(0.0f,10.0f);
	Vertex[2].UV = TVector2(10.0f,10.0f);
	Vertex[3].UV = TVector2(10.0f,0.0f);

	pIndexBuffer = new RIndexBuffer();
	pIndexBuffer->nIndices = 2;
	pIndexBuffer->pIndices = new TIndex16[2];
	pIndexBuffer->pIndices[0] = TIndex16(0,1,2);
	pIndexBuffer->pIndices[1] = TIndex16(0,2,3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////