#include "stdafx.h"
#include "SampleWorld.h"

#include "CBox.h"
#include "CDirectionalLight.h"
#include "CCharacter.h"

IMPLEMENT_CLASS(USampleWorld);

USampleWorld::USampleWorld()
{	
}

USampleWorld::~USampleWorld()
{
}

bool USampleWorld::DestroyWorld()
{
	__super::DestroyWorld();

	delete Box;
	delete DirectionalLight;
	delete Character;

	return true;
}
bool USampleWorld::InitializeWorld()
{
	__super::InitializeWorld();

	Box = new CBox(SideType_Inside);
	Box->m_Scale = TVector3(100.0f,100.0f,100.0f);
	Box->UpdateTransform();
	AddThing(Box);

	DirectionalLight = new CDirectionalLight();
	DirectionalLight->m_Location = TVector3(0.0f, 0.0f, 0.0f);
	DirectionalLight->UpdateTransform();
	AddThing(DirectionalLight);

	Character = new CCharacter();
	Character->UpdateTransform();
	AddThing(Character);

	return true;
}

bool USampleWorld::Tick(DWORD dTime)
{
	__super::Tick(dTime);
	return true;
}