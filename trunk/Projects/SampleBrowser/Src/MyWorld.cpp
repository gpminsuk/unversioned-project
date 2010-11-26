#include "stdafx.h"
#include "MyWorld.h"

#include "CTank.h"
#include "CTerrain.h"

UMyWorld::UMyWorld()
{

}

UMyWorld::~UMyWorld()
{

}

bool UMyWorld::DestroyWorld()
{
	delete Tank[0];
	delete Tank[1];

	delete Terrain;
	return true;
}

bool UMyWorld::InitializeWorld()
{
	m_pWorldData = new TWorldOctree();

	Terrain = new CTerrain();
	AddThing(Terrain);

	Tank[0] = new CTank();
	Tank[1] = new CTank();

	Tank[0]->m_Location = TVector3(10, 0, 10);
	Tank[0]->UpdateTransform();
	Tank[1]->m_Location = TVector3(0, 0, 0);
	Tank[1]->UpdateTransform();

	Tank[0]->IsInTurn = true;

	AddThing(Tank[0]);
	AddThing(Tank[1]);
	return true;
}