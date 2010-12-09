#include "stdafx.h"
#include "BPrimitive.h"
#include "CTank.h"
#include "CBoxComponent.h"
#include "CSkeletalMeshComponent.h"
#include "CWaveIODriver.h"
#include "UWorld.h"
#include "MyWorld.h"
#include "CCylinderCollisionBody.h"
#include "CMissile.h"
#include "CArrow.h"

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

CTank::CTank(TVector3 _rot, float _radian, float _size, int i) :
IsInTurn(false), m_nGage(0)
{
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
	m_Arrow->SetSize(0.03f);
	m_nGage=0;
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
	GWorld->RemoveThing(m_Missile);
	m_Missile->m_vecStartPos=m_Location;
	m_Missile->UpdateTransform();
	Opponent->StartTurn();
	m_Arrow->SetSize(0.00f);
}

void CTank::Fire()
{
	m_Missile->m_Location=m_Location;
	m_Missile->Init(m_nGage,m_FireAngle,m_Location,m_fDirection);
	GWorld->AddThing(m_Missile);
}

void CTank::SetOpponent(CTank* InOpponent)
{
	Opponent = InOpponent;
}

void CTank::Forword()
{
	m_Location.z += 0.3f*m_fDirection;
	UpdateTransform();
}

void CTank::Backword()
{
	m_Location.z -= 0.3f*m_fDirection;
	UpdateTransform();
}

void CTank::ArrowUp()
{
	m_FireAngle+=2.0f;
	if(m_FireAngle>90)
		m_FireAngle=90.0f;
	m_Arrow->SetQuaternion(TVector3(1.0f, 0.0f, 0.0f), 0.040f*m_fDirection);
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

	if(IsInTurn)
	{	
		if(GKeyMap[VK_LEFT])
		{
			Forword();
		}
		if(GKeyMap[VK_RIGHT])
		{
			Backword();
		}
		if(GKeyMap[VK_UP])
		{
			ArrowUp();
		}
		if(GKeyMap[VK_DOWN])
		{
			ArrowDown();
		}
		if(GKeyMap[VK_SPACE])
		{
			m_nGage+=0.005f;
					
		}
	}
}

void CTank::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{

	if(Event == KEY_Up)
	{
		if(IsInTurn)
		{
			if(Param.Key == VK_SPACE)
			{
				Fire();		
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