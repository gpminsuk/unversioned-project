#include "stdafx.h"
#include "MyWorld.h"
#include "CTank.h"
#include "CArrow.h"
#include "CTerrain.h"
#include "CTankManager.h"
#include "CMissile.h"
#include "CCameraViewport.h"
#include "BCamera.h"
#include "CWaveIODriver.h"
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
	soundtime=0;
	
}

UMyWorld::~UMyWorld()
{
	
	m_Network->Closesocket();
}

bool UMyWorld::DestroyWorld()
{
	m_Network->Netsend(3,0,0,0,0);
	m_Network->Netsend(4,0,0,0,0);
//	delete Terrain;
	delete WaitingLayer;
	delete WinLayer;
	delete LossLayer;
	return true;
}
bool UMyWorld::InitializeWorld()
{
	m_Network->InitializeNet();
	GWorld = this;
	m_pWorldData = new TWorldOctree();
	WaitingLayer = new CUIButtonComponent(5);
	WinLayer=	new CUIButtonComponent(6);
	LossLayer= new CUIButtonComponent(7);
	//배경
	m_pTerrain[0] = new CMeshTerrain(TVector3(0.0f, 0.0f, 1.0f), 1.5707963267948966f  , 0.3f, 2);
	m_pTerrain[1] = new CMeshTerrain(TVector3(0.0f, 0.0f, 1.0f), 1.5707963267948966f  , 0.3f, 2);
	m_pTerrain[0]->m_Location.z+=60.0f;
	m_pTerrain[1]->m_Location.z-=80.0f;
	AddThing(m_pTerrain[0]);
	AddThing(m_pTerrain[1]);
	m_pTerrain[0]->UpdateTransform();
	m_pTerrain[1]->UpdateTransform();

	m_pSphere=new CMeshTerrain(TVector3(0.0f, 0.0f, 1.0f), 1.5707963267948966f  , 0.1f, 4);
	m_pSphere->m_Location.x-=80.0f;
	m_pSphere->SetQuaternion(TVector3(0.0f, 1.0f, 0.0f),220.0f/360.0f*3.141592f);
	AddThing(m_pSphere);
	m_pSphere->UpdateTransform();
	
	//////////////////////////////////////////////////////////////////////////
	m_pTankManager = new CTankManager(this);
	m_pTankManager->InitializeTank(TVector3(5.0f, 10.0f, 80.0f), TVector3(5.0f, 10.0f, -60.0f));
	
	Viewport = (CCameraViewport*)Viewports(0);

	Viewport->m_pCamera->m_Subject=m_pTerrain[1];

	m_pTankManager->GetTank(0)->m_fDirection = -1.f;
	m_pTankManager->GetTank(1)->m_fDirection = 1.f;

	AddThing(m_pTankManager->GetTank(0));
	AddThing(m_pTankManager->GetTank(0)->m_Arrow);

	AddThing(m_pTankManager->GetTank(1));
	AddThing(m_pTankManager->GetTank(1)->m_Arrow);
	GSoundDriver->PlayWAVSound(1);
	BeginScene(WaitingLayer);

	return true;
}

void UMyWorld::Tick(DWORD dTime)
{
	soundtime+=dTime;
	if(soundtime>70000)
	{
		GSoundDriver->PlayWAVSound(1);
		soundtime=0;
	}
	char *temp = m_Network->NetRecv();
	memcpy((char *)&strmsg,temp,12);
	switch(strmsg.i)
	{
	case 0:
		// 0 일때는 접속 확인 메세지이므로 무시
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
			m_pTankManager->GetTank(Sequence)->StartTurn();
			break;
		case 5:
			m_pTankManager->GetTank(Sequence)->EndTurn();
			break;
		case 6:
			m_pTankManager->GetTank(Sequence)->Forword();
			break;
		case 7:
			m_pTankManager->GetTank(Sequence)->Backword();
			break;
		case 8:
			m_pTankManager->GetTank(Sequence)->ArrowUp();
			break;
		case 9:
			m_pTankManager->GetTank(Sequence)->ArrowDown();
			break;
		}
		break;
	case 2:
		m_pTankManager->GetTank(strmsg.q)->Fire(strmsg.power,strmsg.angle);
		break;
	case 3:
		EndScene(WaitingLayer);
		break;
	case 4:
		BeginScene(WaitingLayer);
		break;
	}
	temp[0]=0;
	strmsg.i=0;
	return UWorld::Tick(dTime);
}

void UMyWorld::NetTick( DWORD dTime )
{
	
}

void UMyWorld::BeginScene( CUIButtonComponent* _Layer )
{
	for(unsigned int i=0;i<Viewports.Size();++i)
	{
		BViewport* Viewport = Viewports(i);
		Viewport->Render(_Layer);
	}
}

void UMyWorld::EndScene( CUIButtonComponent* _Layer )
{
	for(unsigned int i=0;i<Viewports.Size();++i)
	{
		BViewport* Viewport = Viewports(i);
		Viewport->RemoveRender(_Layer);
	}
}