#pragma once

#include "UWorld.h"

#pragma comment(lib, "vld.lib")
#pragma comment(lib, "uniquecore.lib")
#pragma comment(lib, "uniqueui.lib")

struct myMSG
{
	char i;
	char j;
	char q;
	char p;
	float angle;
	float power;
};

class UMyWorld : public UWorld
{
public:
	UMyWorld();
	~UMyWorld();
	
	CCameraViewport* Viewport;

	class CUIButtonComponent*	UILabel;
	class CTerrain*		Terrain;
	class CTankManager* m_pTankManager;
	class CMeshTerrain* m_pTerrain[4];
	class CNetWork* m_Network;

	int NetworkID;
	int Sequence;
	myMSG strmsg;
	

	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
	virtual void Tick(DWORD dTime);
	virtual void NetTick(DWORD dTime);
	
};