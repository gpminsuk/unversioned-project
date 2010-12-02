#include "stdafx.h"
#include "BPrimitive.h"
#include "CTank.h"
#include "CBoxComponent.h"
#include "CSkeletalMeshComponent.h"
#include "CWaveIODriver.h"

CTank::CTank() :
	IsInTurn(false)
{
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent();
	for(int i=0;i<SkeletalMeshComponent->Primitives.Size();++i)
	{
		SkeletalMeshComponent->Primitives(i)->Translation = TVector3(0.0f,1.0f,-4.3f);
	}
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
			m_Location.x += 0.01f * dTime;
			UpdateTransform();
		}
		if(GKeyMap[VK_RIGHT])
		{
			m_Location.x -= 0.01f * dTime;
			UpdateTransform();
		}
	}
}

void CTank::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	
}

void CTank::PhysicsTick(unsigned long dTime)
{

}

void CTank::UpdateTransform()
{
	for(unsigned int i=0;i<Components.Size();++i)
	{
		for(unsigned int j=0;j<Components(i)->Primitives.Size();++j)
		{
			Components(i)->Primitives(j)->TM = TMatrix(TVector3(m_Location.x, m_Location.y, m_Location.z) + Components(i)->Primitives(j)->Translation,
				TQuaternion(TVector3(1.0f,0.0f,0.0f),1.570796325f), 0.1f);
			//Components(i)->Primitives(j)->TM._41 = m_Location.x;
			//Components(i)->Primitives(j)->TM._42 = m_Location.y;
			//Components(i)->Primitives(j)->TM._43 = m_Location.z;
		}
	}	
}