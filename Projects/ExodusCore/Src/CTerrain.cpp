#include "StdAfx.h"
#include "CTerrain.h"
#include "CTerrainComponent.h"

#include "CHeightFieldCollisionBody.h"

#include "BCamera.h"

CTerrain::CTerrain() {
    CTerrainComponent* TerrainComponent = new CTerrainComponent();
    Components.AddItem(TerrainComponent);

    CHeightFieldCollisionBody* TerrainCollisionBody =
        new CHeightFieldCollisionBody(this);
    CollisionBodies.AddItem(TerrainCollisionBody);

    UpdateTransform();
}

CTerrain::~CTerrain(void) {
}

bool CTerrain::Tick(unsigned long dTime) {
    return BThing::Tick(dTime);
}

void CTerrain::PhysicsTick(unsigned long dTime) {
}

void CTerrain::UpdateTransform() {
    CollisionBodyBounds.Box.Extent = TVector3(5.0f, 5.0f, 5.0f);
    CollisionBodyBounds.Position = m_Location;
    CollisionBodyBounds.Sphere.Radius = 5.0f;
}
