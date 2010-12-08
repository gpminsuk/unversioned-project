#include "stdafx.h"
#include "MyWorld.h"

#include "CTank.h"
#include "CArrow.h"
#include "CTerrain.h"
#include "CTankManager.h"
#include "CMissile.h"
#include "CCameraViewport.h"
#include "BCamera.h"
#include "CMeshTerrain.h"
//#include "vld.h"

extern UWorld* GWorld;

UMyWorld::UMyWorld()
{
	NetworkID =0;
}

UMyWorld::~UMyWorld()
{
}

bool UMyWorld::DestroyWorld()
{
//	delete Terrain;
	return true;
}
bool UMyWorld::InitializeWorld()
{
	GWorld = this;
	m_pWorldData = new TWorldOctree();
	
	//배경
	//Terrain = new CTerrain();
	//AddThing(Terrain);
	m_pTerrain = new CMeshTerrain(TVector3(0.0f, 0.0f, 1.0f), 1.5707963267948966f  , 0.2f, 2);
	
	AddThing(m_pTerrain);
	m_pTerrain->UpdateTransform();
	/*
	pixelshader.fx
	LightPosition이 라이트위치
	LightRadius가 빛반경
	LightBrightness가 밝기
	*/
	m_pTankManager = new CTankManager();
	m_pTankManager->InitializeTank(TVector3(5.0f, 10.0f, 45.0f), TVector3(5.0f, 10.0f, 10.0f));
	
	CCameraViewport* Viewport = (CCameraViewport*)Viewports(0);

	Viewport->m_pCamera->m_Subject=m_pTerrain;

	m_pTankManager->GetTank(0)->m_fDirection = -1.f;
	m_pTankManager->GetTank(1)->m_fDirection = 1.f;


	AddThing(m_pTankManager->GetTank(0));
	AddThing(m_pTankManager->GetTank(0)->m_Arrow);

	AddThing(m_pTankManager->GetTank(1));
	AddThing(m_pTankManager->GetTank(1)->m_Arrow);

	

 	m_pTankManager->GetTank(1)->StartTurn();

	return true;
}

void UMyWorld::Tick(DWORD dTime)
{
	if(GKeyMap[VK_F5])
	{
		m_pTankManager->GetTank(NetworkID++%2)->EndTurn();	
		
	}
	return UWorld::Tick(dTime);
}