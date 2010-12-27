#pragma once

#include "UWorld.h"

#pragma comment(lib, "vld.lib")

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
	DECLARE_CLASS(UMyWorld,);
public:
	UMyWorld();
	~UMyWorld();
	
	CCameraViewport* Viewport;

	class CUIButtonComponent*	WaitingLayer;
	class CUIButtonComponent*	WinLayer;
	class CUIButtonComponent*	LossLayer;

	class CTerrain*		Terrain;
	class CTankManager* m_pTankManager;
	class CMeshTerrain* m_pTerrain[2];
	class CMeshTerrain* m_pSphere;
	class CNetWork* m_Network;

	int NetworkID;
	int Sequence;
	myMSG strmsg;
	DWORD soundtime;

	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
	virtual bool Tick(DWORD dTime);
	virtual void NetTick(DWORD dTime);

	void BeginScene(CUIButtonComponent* _Layer);
	void EndScene(CUIButtonComponent* _Layer);
	
};