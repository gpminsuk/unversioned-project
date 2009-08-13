#include "StdAfx.h"
#include "CTerrain.h"
#include "CTerrainPrimitive.h"

CTerrain::CTerrain(void)
{
	CTerrainPrimitive* TerrainPrimitive = new CTerrainPrimitive();
	Primitives.AddItem(TerrainPrimitive);

	TerrainPrimitive->CreateTerrainPrimitive(10, 10, 1, 1);
}

CTerrain::~CTerrain(void)
{
}

void CTerrain::Tick(unsigned long dTime)
{

}