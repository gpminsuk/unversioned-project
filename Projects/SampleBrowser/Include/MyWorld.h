#pragma once

#include "UWorld.h"

#pragma comment(lib, "vld.lib")

class UMyWorld : public UWorld
{
public:
	UMyWorld();
	~UMyWorld();

	class CTerrain*		Terrain;
	class CTankManager* m_pTankManager;
	class CMeshTerrain* m_pTerrain;

	int NetworkID;

	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
	virtual void Tick(DWORD dTime);
	
};