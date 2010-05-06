#include "StdAfx.h"
#include "CTerrain.h"
#include "CTerrainComponent.h"

#include "CHeightFieldCollisionBody.h"

#include "CCamera.h"

CTerrain::CTerrain(CCamera* Camera)
{
	m_pCamera = Camera;

	CTerrainComponent* TerrainComponent = new CTerrainComponent(this, m_pCamera);
	Components.AddItem(TerrainComponent);

	CHeightFieldCollisionBody* TerrainCollisionBody = new CHeightFieldCollisionBody(this);
	CollisionBodies.AddItem(TerrainCollisionBody);

	UpdateTransform();
}

CTerrain::~CTerrain(void)
{
}

void CTerrain::Tick(unsigned long dTime)
{
	BThing::Tick(dTime);
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