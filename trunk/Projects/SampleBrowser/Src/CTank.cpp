#include "stdafx.h"
#include "BPrimitive.h"
#include "CTank.h"
#include "CBoxComponent.h"
#include "CSkeletalMeshComponent.h"
#include "CWaveIODriver.h"

CTank::CTank() :
	IsInTurn(false), m_nGage(0)
{
	//CBoxComponent* box = new CBoxComponent();
	//Components.AddItem(box);
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(0);
	Components.AddItem(SkeletalMeshComponent);
}

CTank::~CTank()
{

}

void CTank::StartTurn()
{
	GSoundDriver->PlayWAVSound();
	IsInTurn = true;
	TurnTimeLeft = 5000;
}

void CTank::EndTurn()
{
	IsInTurn = false;
	Opponent->StartTurn();
}

void CTank::Fire()
{
	
}

void CTank::SetOpponent(CTank* InOpponent)
{
	Opponent = InOpponent;
}

void CTank::Tick(unsigned long dTime)
{
	if(IsInTurn)
	{
		TurnTimeLeft -= dTime;
		if(TurnTimeLeft <= 0)
		{
			EndTurn();
		}
		if(GKeyMap[VK_LEFT])
		{
			m_Location.z += 0.01f * dTime;
			UpdateTransform();
		}
		if(GKeyMap[VK_RIGHT])
		{
			m_Location.z -= 0.01f * dTime;
			UpdateTransform();
		}
		if(GKeyMap[VK_SPACE] && m_nGage < 100)
		{
			m_nGage+=5;
		}
	}
}

void CTank::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	
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
CTank::CTank(TVector3 _rot, float _radian, float _size, int i) :
	IsInTurn(false), m_nGage(0)
{
	//CBoxComponent* box = new CBoxComponent();
	//Components.AddItem(box);
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(i);
	Components.AddItem(SkeletalMeshComponent);
	SetSize(_size);
	SetQuaternion(_rot, _radian);
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