#include "stdafx.h"
#include "BPrimitive.h"
#include "CArrow.h"
#include "CBoxComponent.h"
#include "CSkeletalMeshComponent.h"
#include "CCylinderCollisionBody.h"
#include "UWorld.h"

CArrow::CArrow() :
m_fRadian(0.f),
m_fAccAngle(0.f)
{
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(1);
	Components.AddItem(SkeletalMeshComponent);

	//CCylinderCollisionBody* CylinderCollisionBody = new CCylinderCollisionBody(this);
	//CollisionBodies.AddItem(CylinderCollisionBody);

	m_Location = TVector3(10.0f, 10.0f, 10.0f);
	UpdateTransform();
}

CArrow::~CArrow()
{

}

void CArrow::Tick(unsigned long dTime)
{
	UpdateTransform();
}

void CArrow::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
}

void CArrow::PhysicsTick(unsigned long dTime)
{
	/*
	TVector3 Loc = m_Location;
	float t = dTime/100.f;
	Loc.y -= (float)(t*t*9.8/2.0f);
	TVector3 Hit = GWorld->LineCheck(this, m_Location, Loc).HitPosition;
	if(Hit != TVector3(0,0,0))
		m_Location = Hit;
	else
		m_Location = Loc;
	UpdateTransform();
	*/
}
bool CArrow::SetQuaternion(TQuaternion _rot)
{
	m_qRot = _rot;
	return true;
}
bool CArrow::SetQuaternion(TVector3 _vecRotationCylinder, float _radian)
{
	SetRotationCylinder(_vecRotationCylinder);
	SetRadian(_radian);
	m_qRot.Rotate(m_vecRotationCylinder, m_fRadian);
	return true;
}

bool CArrow::SetSize(float i_fSize)
{
	m_fSize = i_fSize;
	return true;
}

bool CArrow::SetRadian(float i_fRadian)
{
	m_fRadian = i_fRadian;
	return true;
}

bool CArrow::SetRotationCylinder(TVector3 rot)
{
	m_vecRotationCylinder = rot;
	return true;
}

void CArrow::UpdateTransform()
{
	for(unsigned int i=0;i<Components.Size();++i)
	{
		for(unsigned int j=0;j<Components(i)->Primitives.Size();++j)
		{
			Components(i)->Primitives(j)->Translation = m_Location;
			Components(i)->Primitives(j)->TM = TMatrix(TVector3(m_Location.x, m_Location.y, m_Location.z),m_qRot, m_fSize);
		}
	}
}