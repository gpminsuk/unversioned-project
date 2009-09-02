#include "StdAfx.h"
#include "CCharacter.h"

#include "CBoxPrimitive.h"

#include "CCylinderCollisionBody.h"

#include "UWorld.h"

CCharacter::CCharacter()
{
	m_Height		= 0.0f;	
	m_Radius		= 0.0f;	
	m_Weight		= 0.0f;	
	m_WalkSpeed		= 0.0f;
	m_RunSpeed		= 0.0f;
	

	bIsMovable		= false;
	bIsPhysical		= false;
	bIsPassable		= false;	

	m_Location		= TVector3(0, 0, 0);

	CBoxPrimitive* CharacterPrimitive = new CBoxPrimitive();
	Primitives.AddItem(CharacterPrimitive);

	CCylinderCollisionBody* CharacterCollisionBody = new CCylinderCollisionBody(this);
	CollisionBodies.AddItem(CharacterCollisionBody);

	UpdateTransform();
}

CCharacter::~CCharacter()
{

}

void CCharacter::SetCharacterPosition(TVector3 pos)
{
	m_Location = pos;
	UpdateTransform();
}

void CCharacter::Tick(unsigned long dTime)
{
	
}

void CCharacter::PhysicsTick(unsigned long dTime)
{
	TVector3 Loc = m_Location;
	float t = 0.001f;
	Loc.y -= (float)(t*t*9.8/2.0f);
	SetCharacterPosition(Loc);
}

void CCharacter::UpdateTransform()
{
	m_Location = GWorld->LineCheck(TVector3(0,0,0), TVector3(0,0,0)).HitPosition;

	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		Primitives(i)->Translation = m_Location;
		Primitives(i)->TM._41 = m_Location.x;
		Primitives(i)->TM._42 = m_Location.y;
		Primitives(i)->TM._43 = m_Location.z;
	}

	CollisionBodyBounds.Box.Extent = TVector3(5.0f,5.0f,5.0f);
	CollisionBodyBounds.Position = m_Location;
	CollisionBodyBounds.Sphere.Radius = 5.0f;	
}