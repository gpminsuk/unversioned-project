#pragma once
#include "BPrimitive.h"

class TTerrainQuadTreeNode
{
public:
	TTerrainQuadTreeNode();

	enum E_QuadTreeNodeType
	{
		Node_Root,
		Node_LeftTop,
		Node_LeftBottom,
		Node_RightTop,
		Node_RightBottom
	};

	enum E_NeighborType
	{
		NeighborType_Right = 1,
		NeighborType_Left = 2,
		NeighborType_Upper = 4,
		NeighborType_Lower = 8
	};

	class TTerrainQuadTree* ThisTree;

	TTerrainQuadTreeNode* ParentNode;

	TTerrainQuadTreeNode* Node1;
	TTerrainQuadTreeNode* Node2;
	TTerrainQuadTreeNode* Node3;
	TTerrainQuadTreeNode* Node4;

	short Indices[4];
	TVector3 Origin;
	TVector3 Extent;
	TIntPoint Coord;
	bool bIsLODLeaf;
	char NeighborType;
	unsigned int Depth;

	void DestroyNode();
	void CreateTerrainQuadTreeLeaves(float** HeightValue, TIntPoint Coordinate, TTerrainQuadTreeNode* Parent, TTerrainQuadTree* Tree, unsigned int Level, unsigned int NumCellX, E_QuadTreeNodeType NodeType =
			Node_Root);

	void FillLODIndexBuffer(TIndex16** pData);
	void CheckLODLeafNode(TVector3 LODOrigin);
	unsigned int CheckLODSolderNeighbor();

	bool IsNeighborCracked(E_NeighborType Type);

	float GetLODWeight(TVector3 LODOrigin, TVector3 Origin);
};

class TTerrainQuadTree
{
public:
	TTerrainQuadTree();

	class TTerrainPrimitive* Primitive;

	TTerrainQuadTreeNode* Root;
	TIntPoint TreeCoord;

	void FillLODIndexBuffer(TIndex16** pData);

	unsigned int CheckLODLeafNode(TVector3 LODOrigin);
};

class TTerrainPrimitive: public TPrimitive
{
public:
	TTerrainPrimitive();
	virtual ~TTerrainPrimitive();

	unsigned int CellWidth;
	unsigned int CellHeight;
	unsigned int CellStepSize;

	bool bShouldTessellate;
	TVector3 LODOrigin;

	TIndex16 *pLODIndices;
	int nLODIndices;

	void CreateTerrainQuadTree(float** HeightValue, unsigned int CellSizeX, unsigned int CellSizeY, unsigned int MaxTessellationLevel);
	bool Tessellate(TVector3 Origin);

	TTerrainQuadTree** QuadTree;

	unsigned int QuadTreeSizeX;
	unsigned int QuadTreeSizeY;
	private:
};

class CTerrainPrimitive: public BPrimitive
{
public:
	CTerrainPrimitive(void);
	~CTerrainPrimitive(void);

	bool CreateTerrainPrimitive(unsigned int NumCellX, unsigned int NumCellY, unsigned int NumPatchX, unsigned int NumPatchY);
	bool DestroyTerrainPrimitive();
	void UpdateTerrainPrimitive(TVector3 _LODOrigin);

	virtual RShaderBase* GetShaderType();
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual void IndexTessellate();
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();

	/////////////////////// Render Thread Function //////////////////////////
	bool Tessellate(TVector3 Origin);
	/////////////////////////////////////////////////////////////////////////

	unsigned int NumCellX;
	unsigned int NumCellY;
	unsigned int NumPatchX;
	unsigned int NumPatchY;

	unsigned int MaxTessellationLevel;

	TVector3 LODOrigin;
};
