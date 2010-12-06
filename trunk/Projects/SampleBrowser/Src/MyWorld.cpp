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

UMyWorld::UMyWorld()
{

}

UMyWorld::~UMyWorld()
{
	for(std::vector<CMissile*>::const_iterator it = m_vecMissile.begin();it != m_vecMissile.end();++it)
	{
		delete *it;
	}
}

bool UMyWorld::DestroyWorld()
{
//	delete Terrain;
	return true;
}

BThing* UMyWorld::AddMissile(float fPower,float fAngle,TVector3 & vecPos,float fDirection)
{
	CMissile* Missile = new CMissile;	
	Missile->Init(fPower,fAngle,vecPos,fDirection);
	Missile->m_Location = vecPos;

	AddThing(Missile);
	Missile->UpdateTransform();
	m_vecMissile.push_back(Missile);
	return Missile;
}

bool UMyWorld::InitializeWorld()
{
	m_pWorldData = new TWorldOctree();
	
	//배경
	//Terrain = new CTerrain();
	//AddThing(Terrain);
	m_pTerrain = new CMeshTerrain(TVector3(0.0f, 0.0f, 1.0f), 1.5707963267948966f  , 0.1f, 2);
	AddThing(m_pTerrain);
	m_pTerrain->UpdateTransform();
	
	/*
	pixelshader.fx
	LightPosition이 라이트위치
	LightRadius가 빛반경
	LightBrightness가 밝기
	*/

	m_pVirtualTank	= new CTank(TVector3(0.0f, 0.0f, 0.0f), 0, 0.5f, 0);
	m_pVirtualTank->m_Location = TVector3(10.0f, 0.0f, 25.0f);

	m_pTankManager = new CTankManager();
	m_pTankManager->InitializeTank(TVector3(5.0f, 10.0f, 45.0f), TVector3(5.0f, 10.0f, 10.0f));
	m_pTankManager->SetStartTank(0);

	m_pArrow		= new CArrow;
	m_pArrow->m_Location = m_pTankManager->GetTank(0)->m_Location;
	m_pArrow->m_Location.y += 5;
	m_pArrow->SetSize(0.1f);
	m_pArrow->UpdateTransform();

	CCameraViewport* Viewport = (CCameraViewport*)Viewports(0);
	Viewport->m_pCamera->m_Subject = m_pVirtualTank;

	m_pTankManager->GetTank(0)->m_fDirection = -1.f;
	m_pTankManager->GetTank(1)->m_fDirection = 1.f;


	AddThing(m_pTankManager->GetTank(0));
	AddThing(m_pTankManager->GetTank(1));
	AddThing(m_pArrow);

	if(m_pArrow)
	{
		m_pArrow->SetTank(m_pTankManager->GetTank(0),0);
		m_pArrow->SetTank(m_pTankManager->GetTank(1),1);
	}
	return true;
}

void UMyWorld::Tick(DWORD dTime)
{
	for(std::vector<CMissile*>::const_iterator it = m_vecMissile.begin();it != m_vecMissile.end();)
	{
		CMissile * pMissile = *it;
		if(pMissile->m_Location.y < -100)
		{
			RemoveThing(pMissile);
			//delete *it;
			it = m_vecMissile.erase(it);
		}
		else
		{
			++it;
		}
	}

	UWorld::Tick(dTime);
}