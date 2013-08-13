#include "stdafx.h"
#include "UOneThingWorld.h"

#include "CBox.h"
#include "CDirectionalLight.h"
#include "CCharacter.h"

IMPLEMENT_CLASS(UOneThingWorld);

UOneThingWorld::UOneThingWorld()
{	
}

UOneThingWorld::~UOneThingWorld()
{
}

bool UOneThingWorld::DestroyWorld()
{
	__super::DestroyWorld();
	return true;
}

bool UOneThingWorld::Tick(DWORD dTime)
{
	__super::Tick(dTime);
	return true;
}