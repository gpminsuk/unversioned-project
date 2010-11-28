#pragma once

#include "UWorld.h"

class UMyWorld : public UWorld
{
public:
	UMyWorld();
	~UMyWorld();

	class CTerrain* Terrain;
	class CTank* Tank[2];
	//class CMissile* Missile;

	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
};