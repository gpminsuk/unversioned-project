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
	
	//배경
	Terrain = new CTerrain();
	AddThing(Terrain);
	/*
	pixelshader.fx
	LightPosition이 라이트위치
	LightRadius가 빛반경
	LightBrightness가 밝기
	*/

	m_pVirtualTank = new CTank(TVector3(0.0f, 0.0f, 0.0f), 0, 0.5f, 0);
	m_pVirtualTank->m_Location = TVector3(10.0f, 0.0f, 25.0f);

	m_pTankManager = new CTankManager();
	m_pTankManager->InitializeTank(TVector3(5.0f, 10.0f, 45.0f), TVector3(5.0f, 10.0f, 10.0f));
	m_pTankManager->SetStartTank(0);

	CCameraViewport* Viewport = (CCameraViewport*)Viewports(0);
	Viewport->m_pCamera->m_Subject = m_pVirtualTank;

	AddThing(m_pTankManager->GetTank(0));
	AddThing(m_pTankManager->GetTank(1));
	//AddThing(m_pTankManager->GetGo(0));
	return true;
}