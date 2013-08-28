#include "stdafx.h"
#include "SampleWorld.h"

#include "CWindowsViewport.h"
#include "CBox.h"
#include "CDirectionalLight.h"
#include "CCharacter.h"

IMPLEMENT_CLASS(USampleWorld);

USampleWorld::USampleWorld()
{
}

void USampleWorld::Create(UWorld* World) {
	Box = new CBox(SideType_Inside);
	Box->m_Scale = TVector3(100.0f,100.0f,100.0f);
	Box->UpdateTransform();
	//AddThing(Box);

	DirectionalLight = new CDirectionalLight();
	DirectionalLight->m_Location = TVector3(0.0f, 0.0f, 0.0f);
	DirectionalLight->UpdateTransform();
	//AddThing(DirectionalLight);

	Character = new CCharacter();
	Character->UpdateTransform();
	World->AddThing(Character);
}

USampleWorld::~USampleWorld()
{
	delete Box;
	delete DirectionalLight;
	delete Character;
}