#include "stdafx.h"
#include "BPrimitive.h"
#include "CTank.h"
#include "CBoxComponent.h"
#include "CSkeletalMeshComponent.h"
#include "CWaveIODriver.h"
#include "UWorld.h"
#include "CCylinderCollisionBody.h"
#include "CMissile.h"
#include "CArrow.h"
#include "CCameraViewport.h"
#include "BCamera.h"
#include "CNetWork.h"

extern UWorld* GWorld;

CTank::CTank() :
	IsInTurn(false), m_nGage(0) , m_fDirection(1.f)
{
	//CBoxComponent* box = new CBoxComponent();
	//Components.AddItem(box);
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(0);
	Components.AddItem(SkeletalMeshComponent);

	CCylinderCollisionBody* CylinderCollisionBody = new CCylinderCollisionBody(this);
	CollisionBodies.AddItem(CylinderCollisionBody);
}

CTank::CTank(UMyWorld *world,TVector3 _rot, float _radian, float _size, int i) :
IsInTurn(false), m_nGage(0)
{
	MyWorld=world;
	//CBoxComponent* box = new CBoxComponent();
	//Components.AddItem(box);
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(i);
	Components.AddItem(SkeletalMeshComponent);

	CCylinderCollisionBody* CylinderCollisionBody = new CCylinderCollisionBody(this);
	CollisionBodies.AddItem(CylinderCollisionBody);
	
	m_Arrow = new CArrow();
	m_Arrow->m_Location = m_Location;
	m_Arrow->m_Location.y += 20;
	m_Arrow->SetSize(0.0f);
	m_Arrow->SetQuaternion(TVector3(0.0f, 0.0f, 1.0f),(float)180/360.0f*3.141592f);
	m_Arrow->SetQuaternion(TVector3(1.0f, 0.0f, 0.0f), (float)180/360.0f*3.141592f);
	m_Arrow->UpdateTransform();
	m_Arrow->m_initRot=m_Arrow->m_qRot;
	m_FireAngle = 0.0f;
	m_nGage = 0.0f;
	
	SetSize(_size);
	SetQuaternion(_rot, _radian);

	m_Missile = new CMissile(m_Location);
	m_Missile->Owner = this;
}

CTank::~CTank()
{
}

void CTank::Init()
{
	m_Arrow->m_qRot=m_Arrow->m_initRot;
	m_FireAngle = 0.0f;
	m_nGage=0.0f;
	m_Arrow->SetSize(0.03f);
	
}

void CTank::StartTurn()
{
	//GSoundDriver->PlayWAVSound();
	Init();
	IsInTurn = true;
} 

void CTank::EndTurn()
{
	IsInTurn = false;
	m_Missile->m_vecStartPos=m_Location;
	m_Missile->UpdateTransform();
	m_Arrow->SetSize(0.00f);	
}

void CTank::Fire(float _m_nGage,float _m_FireAngle)
{
	MyWorld->RemoveThing(m_Missile);
	m_Missile->m_Location=m_Location;
	m_Missile->Init(_m_nGage,_m_FireAngle,m_Location,m_fDirection);
	MyWorld->AddThing(m_Missile);
	MyWorld->Viewport->m_pCamera->m_Subject=m_Missile;
	EndTurn();
	Opponent->StartTurn();
}

void CTank::SetOpponent(CTank* InOpponent)
{
	Opponent = InOpponent;
}

void CTank::Forword()
{
	m_Location.z += 0.3f;
	UpdateTransform();
}

void CTank::Backword()
{
	m_Location.z -= 0.3f;
	UpdateTransform();
}

void CTank::ArrowUp()
{
	m_FireAngle+=2.0f;
	if(m_FireAngle>90)
		m_FireAngle=90.0f;
	m_Arrow->SetQuaternion(TVector3(1.0f, 0.0f, 0.0f), 0.040f*m_fDirection);
	MyWorld->m_Network->Netsend(1,8,(char)MyWorld->Sequence,0.0,0.0);	
	UpdateTransform();
}

void CTank::ArrowDown()
{
	m_FireAngle-=2.0f;
	if(m_FireAngle<0)
		m_FireAngle=0.0f;
	m_Arrow->SetQuaternion(TVector3(1.0f, 0.0f, 0.0f), -0.040f*m_fDirection);
	UpdateTransform();
}
void CTank::Tick(unsigned long dTime)
{
	m_Arrow->m_Location = m_Location;
	m_Arrow->m_Location.y += 3;
	//m_Arrow->m_Location.z += m_fDirection*1;
	if(m_Missile->m_Location.y<-100.0f)
	{
		m_Missile->m_Location=Opponent->m_Location;
		MyWorld->RemoveThing(m_Missile);
	}
	if(MyWorld->NetworkID==MyWorld->Sequence && IsInTurn)
	{	
		if(GKeyMap[VK_LEFT])
		{
			Forword();
			MyWorld->m_Network->Netsend(1,6,(char)MyWorld->Sequence,0.0,0.0);	
		}
		if(GKeyMap[VK_RIGHT])
		{
			Backword();
			MyWorld->m_Network->Netsend(1,7,(char)MyWorld->Sequence,0.0,0.0);	
		}
		if(GKeyMap[VK_UP])
		{
			ArrowUp();
			
		}
		if(GKeyMap[VK_DOWN])
		{
			ArrowDown();
			MyWorld->m_Network->Netsend(1,9,(char)MyWorld->Sequence,0.0,0.0);	
		}
		if(GKeyMap[VK_SPACE])
		{
			m_nGage+=0.001f;		
		}
	}
}

void CTank::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	if(Event == KEY_Up)
	{
		if(Param.Key == VK_SPACE)
		{
			GSoundDriver->PlayWAVSound(0);
		}
		if(MyWorld->NetworkID==MyWorld->Sequence && IsInTurn)
		{
			if(Param.Key == VK_SPACE)
			{
				MyWorld->m_Network->Netsend(2,0,MyWorld->Sequence,m_FireAngle,m_nGage);	
				Fire(m_nGage,m_FireAngle);
				MyWorld->m_Network->Netsend(1,3-MyWorld->NetworkID,0,0.0,0.0);
				MyWorld->m_Network->Netsend(1,4,0,0.0,0.0);
				
				if(MyWorld->Sequence == 0)
					MyWorld->Sequence = 1;
				else
					MyWorld->Sequence = 0;				
			}
		}
	}
}
void CTank::PhysicsTick(unsigned long dTime)
{
	
}

bool CTank::SetSize(float i_fSize)
{
	m_fSize = i_fSize;
	return true;
}

bool CTank::SetRadian(float i_fRadian)
{
	m_fRadian = i_fRadian;
	return true;
}

bool CTank::SetRotationCylinder(TVector3 rot)
{
	m_vecRotationCylinder = rot;
	return true;
}

bool CTank::SetQuaternion(TQuaternion _rot)
{
	m_qRot = _rot;
	return true;
}
bool CTank::SetQuaternion(TVector3 _vecRotationCylinder, float _radian)
{
	SetRotationCylinder(_vecRotationCylinder);
	SetRadian(_radian);
	m_qRot.Rotate(m_vecRotationCylinder, m_fRadian);

	m_Arrow->SetRotationCylinder(_vecRotationCylinder);
	m_Arrow->SetRadian(_radian);
	m_Arrow->m_qRot.Rotate(m_vecRotationCylinder, m_fRadian);
	
	m_Arrow->m_initRot=m_Arrow->m_qRot;

	return true;
}
void CTank::UpdateTransform()
{
	for(unsigned int i=0;i<Components.Size();++i)
	{
		for(unsigned int j=0;j<Components(i)->Primitives.Size();++j)
		{
			Components(i)->Primitives(j)->Translation = m_Location;
			Components(i)->Primitives(j)->TM = TMatrix(TVector3(m_Location.x, m_Location.y, m_Location.z),
				m_qRot, m_fSize);
			//Components(i)->Primitives(j)->TM._41 = m_Location.x;
			//Components(i)->Primitives(j)->TM._42 = m_Location.y;
			//Components(i)->Primitives(j)->TM._43 = m_Location.z;
		}
	}	
}