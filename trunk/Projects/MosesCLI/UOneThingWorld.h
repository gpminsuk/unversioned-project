#pragma once

#define generic GENERIC
#include "UWorld.h"
#undef GENERIC

class UOneThingWorld : public UWorld
{
	DECLARE_CLASS(UOneThingWorld,);
public:
	UOneThingWorld();
	~UOneThingWorld();

	class BThing* Thing;
	
	virtual bool DestroyWorld();
	virtual bool Tick(DWORD dTime);	
};