#pragma once
#include "BPrimitive.h"

class TTerrainQuadTreeNode
{
public:
	TTerrainQuadTreeNode() : Node1(0),Node2(0),Node3(0),Node4(0) {}
	TTerrainQuadTreeNode* Node1;
	TTerrainQuadTreeNode* Node2;
	TTerrainQuadTreeNode* Node3;
	TTerrainQuadTreeNode* Node4;

	short Indices[4];

	void DestroyNode();
};

class TTerrainQuadTree
{
public:
	TTerrainQuadTreeNode* Root;
};

class TTerrainPrimitive : public TPrimitive
{
public:
	TTerrainPrimitive();
	~TTerrainPrimitive();

	virtual void Render(TBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();

	unsigned int CellWidth;
	unsigned int CellHeight;
private:
	TIndex16 *pLODIndices;
	int nLODIndices;

	float GetLODLevel(TVector3 Origin, TVector3 Dest);

	bool Tessellate(TVector3 Origin);
};

class CTerrainPrimitive : public BPrimitive
{
public:
	CTerrainPrimitive(void);
	~CTerrainPrimitive(void);

	bool CreateTerrainPrimitive(unsigned int NumCellX, unsigned int NumCellY, unsigned int NumPatchX, unsigned int NumPatchY);
	bool DestroyTerrainPrimitive();

	/////////////////////// Render Thread Function //////////////////////////
	bool Tessellate(TVector3 Origin);
	/////////////////////////////////////////////////////////////////////////

	unsigned int NumCellX;
	unsigned int NumCellY;
	unsigned int NumPatchX;
	unsigned int NumPatchY;

	unsigned int MaxTessellationLevel;

	TTerrainQuadTree** QuadTree;
	unsigned int QuadTreeSizeX;
	unsigned int QuadTreeSizeY;
};
