#include "StdAfx.h"
#include "CCharacter.h"
#include "CBoxPrimitive.h"

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

	//TODO: 곧 추가예정(바뀔수도있음)
	//CharacterPrimitive->CreateBoxPrimitive(m_Location);
}

CCharacter::~CCharacter()
{

}

void CCharacter::SetCharacterPosition(TVector3 pos)
{
	m_Location = pos;
}

void CCharacter::Tick(unsigned long dTime)
{
	
}