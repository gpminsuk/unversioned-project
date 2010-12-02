#pragma once

#include "UWorld.h"

#pragma comment(lib, "vld.lib")

class UMyWorld : public UWorld
{
public:
	UMyWorld();
	~UMyWorld();

	class CTerrain* Terrain;
	class CTank* m_pVirtualTank;
	class CTankManager* m_pTankManager;

	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
};