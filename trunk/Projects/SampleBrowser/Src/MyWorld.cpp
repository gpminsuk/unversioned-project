#include "stdafx.h"
#include "MyWorld.h"

#include "CTank.h"
#include "CTerrain.h"
#include "CMissile.h"
#include "CCameraViewport.h"
#include "BCamera.h"

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
	delete Missile;

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
	Missile = new CMissile();

	Tank[0]->m_Location = TVector3(10, 0, 10);
	Tank[0]->UpdateTransform();

	Tank[1]->m_Location = TVector3(50, 0, 50);
	Tank[1]->UpdateTransform();
	
	Tank[0]->SetOpponent(Tank[1]);
	Tank[1]->SetOpponent(Tank[0]);

	Tank[0]->StartTurn();

	CCameraViewport* Viewport = (CCameraViewport*)Viewports(0);
	Viewport->m_pCamera->m_Subject = Tank[0];

	AddThing(Tank[0]);
	AddThing(Tank[1]);
	AddThing(Missile);
	return true;
}