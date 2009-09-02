#include "StdAfx.h"
#include "CTerrain.h"
#include "CTerrainPrimitive.h"

#include "CHeightFieldCollisionBody.h"

#include "CCamera.h"

CTerrain::CTerrain(CCamera* Camera)
{
	m_pCamera = Camera;

	CTerrainPrimitive* TerrainPrimitive = new CTerrainPrimitive();
	Primitives.AddItem(TerrainPrimitive);

	CHeightFieldCollisionBody* TerrainCollisionBody = new CHeightFieldCollisionBody(this);
	CollisionBodies.AddItem(TerrainCollisionBody);

	TerrainPrimitive->CreateTerrainPrimitive(32, 32, 1, 1);

	UpdateTransform();
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

void CTerrain::PhysicsTick(unsigned long dTime)
{

}

void CTerrain::UpdateTransform()
{
	CollisionBodyBounds.Box.Extent = TVector3(5.0f,5.0f,5.0f);
	CollisionBodyBounds.Position = m_Location;
	CollisionBodyBounds.Sphere.Radius = 5.0f;
}