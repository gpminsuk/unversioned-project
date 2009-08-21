#include "StdAfx.h"
#include "CTerrain.h"
#include "CTerrainPrimitive.h"

#include "CCamera.h"

CTerrain::CTerrain(CCamera* Camera)
{
	m_pCamera = Camera;

	CTerrainPrimitive* TerrainPrimitive = new CTerrainPrimitive();
	Primitives.AddItem(TerrainPrimitive);

	TerrainPrimitive->CreateTerrainPrimitive(32, 32, 1, 1);
}

CTerrain::~CTerrain(void)
{
}

void CTerrain::Tick(unsigned long dTime)
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		CTerrainPrimitive* Prim = dynamic_cast<CTerrainPrimitive*>(Primitives(i));
		if(Prim)
		{
			Prim->UpdateTerrainPrimitive(m_pCamera->m_Location);
		}
	}
}