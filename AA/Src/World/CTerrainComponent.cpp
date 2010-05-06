#include "StdAfx.h"
#include "CTerrainComponent.h"
#include "CTerrainPrimitive.h" 
#include "CCamera.h"

CTerrainComponent::CTerrainComponent(BThing* InOwner, CCamera* Camera)
:	BComponent(InOwner), 
	CurrentCamera(Camera)	
{
	CTerrainPrimitive* TerrainPrimitive = new CTerrainPrimitive();
	Primitives.AddItem(TerrainPrimitive);
	TerrainPrimitive->CreateTerrainPrimitive(32, 32, 1, 1);
}

CTerrainComponent::~CTerrainComponent()
{
}

void CTerrainComponent::UpdateComponent()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		CTerrainPrimitive* Prim = dynamic_cast<CTerrainPrimitive*>(Primitives(i));
		if(Prim && CurrentCamera)
		{
			Prim->UpdateTerrainPrimitive(CurrentCamera->m_Location);
		}
	}
}