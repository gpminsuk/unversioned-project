#include "stdafx.h"
#include "BPrimitive.h"
#include "CMissile.h"
#include "CBoxComponent.h"
#include "CCylinderCollisionBody.h"
#include "CSkeletalMeshComponent.h"
#include "UWorld.h"

extern UWorld* GWorld;

CMissile::CMissile(TVector3 _m_Location) :
m_fPastTime(0) , m_fDirection(1.f)
{
	CSkeletalMeshComponent* SkeletalMeshComponent = new CSkeletalMeshComponent(3);
	Components.AddItem(SkeletalMeshComponent);

	m_Location = _m_Location;
	UpdateTransform();
}

CMissile::~CMissile()
{
	
}

void CMissile::Tick(unsigned long dTime)
{

}

void CMissile::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	
}

void CMissile::PhysicsTick(unsigned long dTime)
{	
	m_fPastTime += dTime/100.f;

	TVector3 Loc = m_Location;
	//float t = dTime/100.f;
	float fSpeed = 3.0f * 10;

	//Loc.y  = 0.5f - (float)(t*t*9.8/2.0f);
	float fG = ((m_fPastTime * m_fPastTime) * 7.9f) / 2.0f;

	Loc.y = m_vecStartPos.y + (fSpeed * m_fPastTime) * 0.5f - fG;
	Loc.z = m_vecStartPos.z + ((m_fPastTime * 7.f) * m_fDirection);

	TVector3 Hit = GWorld->LineCheck(Owner, m_Location, Loc).HitPosition;

	if(Hit != TVector3(0,0,0))
		m_Location = Hit;

	else
		m_Location = Loc;

	UpdateTransform();

	//미사일과 탱그 충돌 일어남
	//미사일 터트리고 상대 탱크 HP 감소


	
}

void CMissile::UpdateTransform()
{
	for(unsigned int i=0;i<Components(0)->Primitives.Size();++i)
	{
		Components(0)->Primitives(i)->Translation = m_Location;
		Components(0)->Primitives(i)->TM._41 = m_Location.x;
		Components(0)->Primitives(i)->TM._42 = m_Location.y;
		Components(0)->Primitives(i)->TM._43 = m_Location.z;

		Components(i)->Primitives(i)->TM = TMatrix(TVector3(m_Location.x, m_Location.y, m_Location.z),
			TQuaternion(TVector3(1.0f,0.0f,0.0f),0.f), 0.02f);
	}
	

	for(unsigned int i=0;i<CollisionBodies.Size();++i)
	{
		for(unsigned int j=0;j<Components(i)->Primitives.Size();++j)
		{
			Components(i)->Primitives(j)->Translation = m_Location;
			Components(i)->Primitives(j)->TM = TMatrix(TVector3(m_Location.x, m_Location.y, m_Location.z),
				TQuaternion(TVector3(1.0f,0.0f,0.0f),0.f), 0.02f);
		}
	}
	
	CollisionBodyBounds.Box.Extent = TVector3(5.0f,5.0f,5.0f);
	CollisionBodyBounds.Position = m_Location;
	CollisionBodyBounds.Sphere.Radius = 5.0f;	
}

void CMissile::Init(float fPower,float fAngle,TVector3& vecStartPosition,float fDirection)
{
	m_fAngle = fAngle;
	m_fPower = fPower;

	m_vecStartPos = vecStartPosition;
	m_fDirection = fDirection;
}