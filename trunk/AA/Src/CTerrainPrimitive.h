#pragma once
#include "BPrimitive.h"

class TTerrainPrimitive : public TPrimitive
{
public:
	TTerrainPrimitive();
	~TTerrainPrimitive();

	virtual void Render(TBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
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
};
