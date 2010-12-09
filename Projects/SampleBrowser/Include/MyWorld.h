#pragma once

#include "UWorld.h"

#pragma comment(lib, "vld.lib")

struct myMSG
{
	char i;
	char j;
	char q;
	float angle;
	float power;
};

class UMyWorld : public UWorld
{
public:
	UMyWorld();
	~UMyWorld();

	class CUIButtonComponent*	UILabel;
	class CTerrain*		Terrain;
	class CTankManager* m_pTankManager;
	class CMeshTerrain* m_pTerrain[2];
	class CNetWork* m_Network;

	int NetworkID;
	int Sequence;
	myMSG strmsg;

	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
	virtual void Tick(DWORD dTime);
	
};