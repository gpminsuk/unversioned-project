#include "StdAfx.h"

#include "RResource.h"

#include "CTerrainPrimitive.h"

TTerrainPrimitive::TTerrainPrimitive()
{

}

void TTerrainPrimitive::Render(TBatch *Batch)
{
	Batch->nVertexStride = pBuffer->m_pVB->nVertexStride;
	Batch->nVertices += pBuffer->m_pVB->nVertices;
	Batch->nIndices += pBuffer->m_pIB->nIndices;
}

unsigned int TTerrainPrimitive::FillDynamicVertexBuffer(char** pData)
{
	memcpy((*pData), pBuffer->m_pVB->pVertices, 
		pBuffer->m_pVB->nVertices * pBuffer->m_pVB->nVertexStride);
	for(int k=0;k<pBuffer->m_pVB->nVertices;++k)
	{
		*((TVector3*)&((*pData)[k*pBuffer->m_pVB->nVertexStride])) = TM.TransformVector3(*((TVector3*)&((*pData)[k*pBuffer->m_pVB->nVertexStride])));
	}
	*pData += pBuffer->m_pVB->nVertices * pBuffer->m_pVB->nVertexStride;
	return pBuffer->m_pVB->nVertices;
}

unsigned int TTerrainPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex)
{
	for(int k=0;k<pBuffer->m_pIB->nIndices;++k)
	{
		TIndex16 tmpIndex;
		tmpIndex._1 = pBuffer->m_pIB->pIndices[k]._1 + *BaseIndex;
		tmpIndex._2 = pBuffer->m_pIB->pIndices[k]._2 + *BaseIndex;
		tmpIndex._3 = pBuffer->m_pIB->pIndices[k]._3 + *BaseIndex;
		(*pData)[k] = tmpIndex;
	}
	*BaseIndex += pBuffer->m_pVB->nVertices;
	*pData += pBuffer->m_pIB->nIndices;

	return pBuffer->m_pVB->nVertices;
}

CTerrainPrimitive::CTerrainPrimitive(void)
: NumCellX(10),
	NumCellY(10),
	NumPatchX(1),
	NumPatchY(1)
{
}

CTerrainPrimitive::~CTerrainPrimitive(void)
{
	DestroyTerrainPrimitive();
}

bool CTerrainPrimitive::CreateTerrainPrimitive(unsigned int NumCellX, unsigned int NumCellY, unsigned int NumPatchX, unsigned int NumPatchY)
{
	DestroyTerrainPrimitive();
	for(unsigned int nPatchX = 0; nPatchX < NumPatchX; ++nPatchX)
	{
		for(unsigned int nPatchY = 0; nPatchY < NumPatchY; ++nPatchY)
		{
			TTerrainPrimitive* Primitive = new TTerrainPrimitive();
			Primitives.AddItem(Primitive);
			Primitive->pBuffer = new RStaticPrimitiveBuffer();

			RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
			Primitive->pBuffer->m_pVB = pVB;

			RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
			Primitive->pBuffer->m_pIB = pIB;

			pVB->Declaration = new VertexDeclaration[2];
			pVB->Declaration[0].Offset = 0;
			pVB->Declaration[0].Type = DECLTYPE_FLOAT3;	// Position
			pVB->Declaration[1].Offset = 12;
			pVB->Declaration[1].Type = DECLTYPE_FLOAT2;	// UV

			struct VD
			{
				TVector3 Pos;
				TVector2 UV;
			};

			unsigned int SizeX = (NumCellX/NumPatchX);
			unsigned int SizeY = (NumCellY/NumPatchY);

			pVB->nVertexStride = sizeof(VD);
			pVB->nVertices = SizeX*SizeY*4;
			pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

			VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);
			for(unsigned int i=0;i<SizeX;++i)	
			{
				for(unsigned int j=0;j<SizeY;++j)
				{
					Vertex[(i*SizeX + j)*4 + 0].Pos = TVector3((float)i    ,(float)j    ,0.0f);
					Vertex[(i*SizeX + j)*4 + 0].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 1].Pos = TVector3((float)i    ,(float)j + 1,0.0f);
					Vertex[(i*SizeX + j)*4 + 1].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 2].Pos = TVector3((float)i + 1,(float)j    ,0.0f);
					Vertex[(i*SizeX + j)*4 + 2].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 3].Pos = TVector3((float)i + 1,(float)j + 1,0.0f);
					Vertex[(i*SizeX + j)*4 + 3].UV = TVector2(0.0f,0.0f);
				}
			}

			pIB->nIndices = SizeX*SizeY*2;
			pIB->pIndices = new TIndex16[pIB->nIndices];
			for(unsigned int i=0;i<SizeX;++i)
			{
				for(unsigned int j=0;j<SizeY;++j)
				{
					pIB->pIndices[(i*SizeX + j)*2 + 0] = TIndex16((i*SizeX + j)*4 + 0, (i*SizeX + j)*4 + 3, (i*SizeX + j)*4 + 1);
					pIB->pIndices[(i*SizeX + j)*2 +	1] = TIndex16((i*SizeX + j)*4 + 0, (i*SizeX + j)*4 + 2, (i*SizeX + j)*4 + 3);
				}
			}
		}
	}	
	return true;
}

bool CTerrainPrimitive::DestroyTerrainPrimitive()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		delete Primitives(i)->pBuffer->m_pIB;
		delete Primitives(i)->pBuffer->m_pVB;
		delete Primitives(i)->pBuffer;
		delete Primitives(i);
	}
	Primitives.Clear();
	return true;
}

///////////////////////////////////////////////////// Render Thread Function //////////////////////////////////////////////////////

bool CTerrainPrimitive::Tessellate(TVector3 Origin)
{
	return true;
}