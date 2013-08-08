#include "StdAfx.h"
#include "CTerrainComponent.h"
#include "CTerrainPrimitive.h"
#include "BCamera.h"

CTerrainComponent::CTerrainComponent() {
    CTerrainPrimitive* TerrainPrimitive = new CTerrainPrimitive();
    Primitives.AddItem(TerrainPrimitive);
    TerrainPrimitive->CreateTerrainPrimitive(32, 32, 1, 1);
}

CTerrainComponent::~CTerrainComponent() {
}

void CTerrainComponent::UpdateComponent() {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
    }
}
