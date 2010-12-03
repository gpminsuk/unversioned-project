#include "stdafx.h"
#include "MyWorld.h"

#include "CTank.h"
#include "CTerrain.h"
#include "CTankManager.h"
#include "CMissile.h"
#include "CCameraViewport.h"
#include "BCamera.h"
//#include "vld.h"

UMyWorld::UMyWorld()
{

}

UMyWorld::~UMyWorld()
{

}

bool UMyWorld::DestroyWorld()
{
	delete Terrain;
	return true;
}

bool UMyWorld::InitializeWorld()
{
	m_pWorldData = new TWorldOctree();
	
	//¹è°æ
	Terrain = new CTerrain();
	Terrain->m_Location = TVector3(0.0f, 0.0f, 100.0f);
	AddThing(Terrain);

	m_pVirtualTank = new CTank(TVector3(1.0f, 0.0f, 0.0f), -1.5707963f, 1.0f);
	m_pVirtualTank->m_Location = TVector3(-10.0f, 0.0f, 0.0f);

	m_pTankManager = new CTankManager();
	m_pTankManager->InitializeTank(TVector3(0.0f, 0.0f, 10.0f), TVector3(0.0f, 0.0f, -10.0f));
	m_pTankManager->SetStartTank(0);

	CCameraViewport* Viewport = (CCameraViewport*)Viewports(0);
	Viewport->m_pCamera->m_Subject = m_pVirtualTank;
	Viewport->m_pCamera->m_Location = TVector3(0.0f, 0.0f, 0.0f);

	AddThing(m_pTankManager->GetTank(0));
	AddThing(m_pTankManager->GetTank(1));
	return true;
}