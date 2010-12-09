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
#include "CNetWork.h"

#include "..\..\UniqueUI\Include\CUIButton.h"

//#include "vld.h"

extern UWorld* GWorld;

UMyWorld::UMyWorld()
{
	NetworkID = 3;
	Sequence = 5;
	m_Network = new CNetWork();
	m_Network->InitializeNet();
}

UMyWorld::~UMyWorld()
{
	
}

bool UMyWorld::DestroyWorld()
{
//	delete Terrain;
	delete UILabel;
	return true;
}
bool UMyWorld::InitializeWorld()
{
	GWorld = this;
	m_pWorldData = new TWorldOctree();
	UILabel = new CUIButtonComponent(4);
	for(unsigned int i=0;i<Viewports.Size();++i)
	{
		BViewport* Viewport = Viewports(i);
		Viewport->Render(UILabel);
	}
	//배경
	//Terrain = new CTerrain();
	//AddThing(Terrain);
	m_pTerrain[0] = new CMeshTerrain(TVector3(0.0f, 0.0f, 1.0f), 1.5707963267948966f  , 0.3f, 2);
	m_pTerrain[1] = new CMeshTerrain(TVector3(0.0f, 0.0f, 1.0f), 1.5707963267948966f  , 0.3f, 2);
	m_pTerrain[0]->m_Location.z+=60.0f;
	m_pTerrain[1]->m_Location.z-=80.0f;
	m_pTerrain[0]->UpdateTransform();
	m_pTerrain[1]->UpdateTransform();
	AddThing(m_pTerrain[0]);
	AddThing(m_pTerrain[1]);
	
	/*
	pixelshader.fx
	LightPosition이 라이트위치
	LightRadius가 빛반경
	LightBrightness가 밝기
	*/
	m_pTankManager = new CTankManager(this);
	m_pTankManager->InitializeTank(TVector3(5.0f, 10.0f, 80.0f), TVector3(5.0f, 10.0f, -60.0f));
	
	CCameraViewport* Viewport = (CCameraViewport*)Viewports(0);

	Viewport->m_pCamera->m_Subject=m_pTerrain[1];

	m_pTankManager->GetTank(0)->m_fDirection = -1.f;
	m_pTankManager->GetTank(1)->m_fDirection = 1.f;

	AddThing(m_pTankManager->GetTank(0));
	AddThing(m_pTankManager->GetTank(0)->m_Arrow);

	AddThing(m_pTankManager->GetTank(1));
	AddThing(m_pTankManager->GetTank(1)->m_Arrow);




	return true;
}

void UMyWorld::Tick(DWORD dTime)
{
	char *temp = m_Network->NetRecv();
	memcpy(&strmsg,temp,11);
	switch(strmsg.i)
	{
	case 0:
		// 0 일때는 접속 확인 메세지이므로 개무시
		break;
	case 1:
		//네트워크 아이디 부여받음
		switch(strmsg.j)
		{
		case 0:
			NetworkID = 0;
			break;
		case 1:
			NetworkID = 1;
			break;
		case 2:
			Sequence = 0;
			break;
		case 3:
			Sequence = 1;
			break;
		case 4:
			m_pTankManager->GetTank(0)->StartTurn();
			break;
		case 5:
			m_pTankManager->GetTank(1)->StartTurn();
			break;
		case 6:
			switch(strmsg.q)
			{
			case 0:
				m_pTankManager->GetTank(0)->Forword();
				break;
			case 1:
				m_pTankManager->GetTank(1)->Forword();
				break;
			}
			break;
		case 7:
			switch(strmsg.q)
			{
			case 0:
				m_pTankManager->GetTank(0)->Backword();
				break;
			case 1:
				m_pTankManager->GetTank(1)->Backword();
				break;
			}
			break;
		case 8:
			switch(strmsg.q)
			{
			case 0:
				m_pTankManager->GetTank(0)->ArrowUp();
				break;
			case 1:
				m_pTankManager->GetTank(1)->ArrowUp();
				break;
			}
			break;
		case 9:
			switch(strmsg.q)
			{
			case 0:
				m_pTankManager->GetTank(0)->ArrowDown();
				break;
			case 1:
				m_pTankManager->GetTank(1)->ArrowDown();
				break;
			}
			break;
		case 10:
			switch(strmsg.q)
			{
			case 0:
				m_pTankManager->GetTank(0)->Fire(strmsg.power,strmsg.angle);
				break;
			case 1:
				m_pTankManager->GetTank(1)->Fire(strmsg.power,strmsg.angle);
				break;
			}
			break;
		}
		break;
	}
	temp[0]=0;//무시 메세지를 만들기위행

	return UWorld::Tick(dTime);
}