#include "stdafx.h"
#include "UMosesWorld.h"

#include "CBox.h"
#include "CDirectionalLight.h"
#include "CCharacter.h"

IMPLEMENT_CLASS(UMosesWorld);

UMosesWorld::UMosesWorld()
{
	Box = new CBox(SideType_Inside);
	Box->m_Scale = TVector3(100.0f,100.0f,100.0f);
	Box->UpdateTransform();
	AddThing(Box);

	DirectionalLight = new CDirectionalLight();
	DirectionalLight->m_Location = TVector3(0.0f, 100.0f, 0.0f);
	DirectionalLight->UpdateTransform();
	//AddThing(DirectionalLight);

	Character = new CCharacter();
	Character->UpdateTransform();
	AddThing(Character);
}

UMosesWorld::~UMosesWorld()
{
}

bool UMosesWorld::DestroyWorld()
{
	__super::DestroyWorld();

	delete Box;
	delete DirectionalLight;
	delete Character;

	return true;
}

bool UMosesWorld::Tick(DWORD dTime)
{
	__super::Tick(dTime);
	return true;
}