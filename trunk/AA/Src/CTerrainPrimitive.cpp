#include "StdAfx.h"

#include "RResource.h"

#include "CTerrainPrimitive.h"

TTerrainPrimitive::TTerrainPrimitive()
: pLODIndices(0)
{

}

void TTerrainPrimitive::Render(TBatch *Batch)
{
	Batch->nVertexStride = pBuffer->m_pVB->nVertexStride;
	Batch->nVertices += pBuffer->m_pVB->nVertices;
}

unsigned int TTerrainPrimitive::GetNumIndices()
{
	return pBuffer->m_pIB->nIndices;
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
	for(unsigned int k=0;k<GetNumIndices();++k)
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

bool TTerrainPrimitive::Tessellate(TVector3 Origin)
{	
	delete pLODIndices;
	for(int k=0;k<pBuffer->m_pVB->nVertices;++k)
	{
		GetLODLevel(Origin, *((TVector3*)(pBuffer->m_pVB->pVertices) + k));
	}
	return true;
}

float TTerrainPrimitive::GetLODLevel(TVector3 Origin, TVector3 Dest)
{
	return (Origin - Dest).SizeSquared();
}

CTerrainPrimitive::CTerrainPrimitive(void)
:	NumCellX(10),
	NumCellY(10),
	NumPatchX(1),
	NumPatchY(1),
	QuadTree(0),
	MaxTessellationLevel(16)
{
}

CTerrainPrimitive::~CTerrainPrimitive(void)
{
	DestroyTerrainPrimitive();
}

bool CTerrainPrimitive::CreateTerrainPrimitive(unsigned int NumCellX, unsigned int NumCellY, unsigned int NumPatchX, unsigned int NumPatchY)
{
	DestroyTerrainPrimitive();

	QuadTreeSizeX = NumCellX/MaxTessellationLevel;
	QuadTreeSizeY = NumCellY/MaxTessellationLevel;

	QuadTree = new TTerrainQuadTree*[QuadTreeSizeX];
	for(unsigned int i=0;i<QuadTreeSizeX;++i)
	{
		QuadTree[i] = new TTerrainQuadTree[QuadTreeSizeY];
		for(unsigned int j=0;j<QuadTreeSizeY;++j)
		{
			QuadTree[i][j].Root = new TTerrainQuadTreeNode();
			QuadTree[i][j].Root->Indices[0] = MaxTessellationLevel*i*4 + j*NumCellX*4;
			QuadTree[i][j].Root->Indices[1] = MaxTessellationLevel*(i+1)*4 + j*NumCellX*4 - 3;
			QuadTree[i][j].Root->Indices[2] = MaxTessellationLevel*i*4 + j*NumCellX*4 + 2;
			QuadTree[i][j].Root->Indices[3] = MaxTessellationLevel*(i+1)*4 + (j+1)*NumCellX*4 - 1;
		}
	}

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

			Primitive->CellHeight = SizeY;
			Primitive->CellWidth = SizeX;

			pVB->nVertexStride = sizeof(VD);
			pVB->nVertices = SizeX*SizeY*4;
			pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

			VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);
			for(unsigned int i=0;i<SizeX;++i)	
			{
				for(unsigned int j=0;j<SizeY;++j)
				{
					/*Vertex[(i*SizeX + j)*4 + 0].Pos = TVector3((float)i    ,(float)j    ,0.0f);
					Vertex[(i*SizeX + j)*4 + 0].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 1].Pos = TVector3((float)i    ,(float)j + 1,0.0f);
					Vertex[(i*SizeX + j)*4 + 1].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 2].Pos = TVector3((float)i + 1,(float)j    ,0.0f);
					Vertex[(i*SizeX + j)*4 + 2].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 3].Pos = TVector3((float)i + 1,(float)j + 1,0.0f);
					Vertex[(i*SizeX + j)*4 + 3].UV = TVector2(0.0f,0.0f);*/
					Vertex[(i*SizeX + j)*4 + 0].Pos = TVector3((float)i    ,0.0f		,(float)j);
					Vertex[(i*SizeX + j)*4 + 0].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 1].Pos = TVector3((float)i    ,0.0f		,(float)j + 1);
					Vertex[(i*SizeX + j)*4 + 1].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 2].Pos = TVector3((float)i + 1,0.0f		,(float)j);
					Vertex[(i*SizeX + j)*4 + 2].UV = TVector2(0.0f,0.0f);
					Vertex[(i*SizeX + j)*4 + 3].Pos = TVector3((float)i + 1,0.0f		,(float)j + 1);
					Vertex[(i*SizeX + j)*4 + 3].UV = TVector2(0.0f,0.0f);
				}
			}

			pIB->nIndices = SizeX*SizeY*2;
			pIB->pIndices = new TIndex16[pIB->nIndices];
			for(unsigned int i=0;i<SizeX;++i)
			{
				for(unsigned int j=0;j<SizeY;++j)
				{
					pIB->pIndices[(i*SizeX + j)*2 + 0] = TIndex16((i*SizeX + j)*4 + 0, (i*SizeX + j)*4 + 1, (i*SizeX + j)*4 + 3);
					pIB->pIndices[(i*SizeX + j)*2 +	1] = TIndex16((i*SizeX + j)*4 + 0, (i*SizeX + j)*4 + 3, (i*SizeX + j)*4 + 2);
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

	if(QuadTree)
	{
		for(unsigned int i=0;i<QuadTreeSizeX;++i)
		{
			for(unsigned int j=0;i<QuadTreeSizeY;++j)
			{
				QuadTree[i][j].Root->DestroyNode();
				delete QuadTree[i][j].Root;
			}
			delete[] QuadTree[i];
		}
		delete[] QuadTree;
	}	

	return true;
}

///////////////////////////////////////////////////// Render Thread Function //////////////////////////////////////////////////////

bool CTerrainPrimitive::Tessellate(TVector3 Origin)
{
	return true;
}

//////////////////////////////////////////////////// Terrain QuadTree /////////////////////////////////////////////////////////////

void TTerrainQuadTreeNode::DestroyNode()
{
	if(Node1)
	{
		Node1->DestroyNode();
		delete Node1;
	}
	if(Node2)
	{
		Node2->DestroyNode();
		delete Node2;
	}
	if(Node3)
	{
		Node3->DestroyNode();
		delete Node3;
	}
	if(Node4)
	{
		Node4->DestroyNode();
		delete Node4;
	}
}