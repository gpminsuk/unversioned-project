#pragma once

#include "UWorld.h"

#pragma comment(lib, "vld.lib")

class CMissile;
class UMyWorld : public UWorld
{
public:
	UMyWorld();
	~UMyWorld();

	class CTerrain*		Terrain;
	class CTank*		m_pVirtualTank;
	class CTankManager* m_pTankManager;
	class CArrow*		m_pArrow;
	class CMeshTerrain* m_pTerrain;
	

	std::vector<CMissile*> m_vecMissile;


	BThing*	AddMissile(float fPower,float fAngle,TVector3 & vecPos,float fDirection);

	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
	virtual void Tick(DWORD dTime);
};