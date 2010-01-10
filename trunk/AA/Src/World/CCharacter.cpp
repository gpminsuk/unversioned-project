#include "StdAfx.h"
#include "CCharacter.h"

#include "CBoxPrimitive.h"
#include "CSkeletalMeshPrimitive.h"

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

	//CSkeletalMeshPrimitive* CharacterPrimitives = new CSkeletalMeshPrimitive(RBoneHierarchy::Bones(0));
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

	m_Location = GWorld->LineCheck(this, m_Location, pos).HitPosition;
	UpdateTransform();
}

void CCharacter::Tick(unsigned long dTime)
{
	if(GKeyMap['W'])
		SetCharacterPosition(m_Location + TVector3(0.0005f,0.0f,0.0f));
	if(GKeyMap['S'])
		SetCharacterPosition(m_Location + TVector3(-0.0005f,0.0f,0.0f));
	if(GKeyMap['A'])
		SetCharacterPosition(m_Location + TVector3(0.0f,0.0f,0.0005f));
	if(GKeyMap['D'])
		SetCharacterPosition(m_Location + TVector3(0.0f,0.0f,-0.0005f));
}

void CCharacter::PhysicsTick(unsigned long dTime)
{
	TVector3 Loc = m_Location;
	float t = 0.003f;
	Loc.y -= (float)(t*t*9.8/2.0f);
	SetCharacterPosition(Loc);
}

void CCharacter::UpdateTransform()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		Primitives(i)->Translation = m_Location;
		Primitives(i)->TM._41 = m_Location.x;
		Primitives(i)->TM._42 = m_Location.y;
		Primitives(i)->TM._43 = m_Location.z;
	}
	for(unsigned int i=0;i<CollisionBodies(0)->Primitives.Size();++i)
	{
		CollisionBodies(0)->Primitives(i)->Translation = m_Location;
		CollisionBodies(0)->Primitives(i)->TM._41 = m_Location.x;
		CollisionBodies(0)->Primitives(i)->TM._42 = m_Location.y;
		CollisionBodies(0)->Primitives(i)->TM._43 = m_Location.z;
	}

	CollisionBodyBounds.Box.Extent = TVector3(5.0f,5.0f,5.0f);
	CollisionBodyBounds.Position = m_Location;
	CollisionBodyBounds.Sphere.Radius = 5.0f;	
}

void CCharacter::InputMouse(EMouse_Event Event, TMouseInput_Param& Param)
{
}

void CCharacter::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
}

void CCharacter::InputChar()
{
}