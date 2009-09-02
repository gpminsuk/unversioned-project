#pragma once

#include <windows.h>
#include "TDataTypes.h"
#include "InputDefine.h"

class AObject;

class BRenderer;

class CCamera;
class CCameraViewport;

class THitInfo
{
public:
	class BThing* Thing;
	TVector3 HitPosition;
};

class TWorldStructure
{
public:
	TWorldStructure() {}
	virtual ~TWorldStructure() {}

	virtual void Tick(unsigned long dTime) = 0;
	virtual void AddThing(class BThing* Thing) = 0;
	virtual void RemoveThing(class BThing* Thing) = 0;

	virtual THitInfo LineCheck(TVector3 Start, TVector3 End, TVector3 Extent = TVector3(0,0,0)) = 0;
};

class TWorldOctree : public TWorldStructure
{
public:
	TWorldOctree();
	~TWorldOctree();

	class TWorldOctreeNode* CollisionBodyRootNode;
	class TWorldOctreeNode* PrimitiveRootNode;

	TArray<class BThing*> AllObjects;

	virtual void Tick(unsigned long dTime);
	virtual void AddThing(class BThing* Thing);
	virtual void RemoveThing(class BThing* Thing);
	virtual THitInfo LineCheck(TVector3 Start, TVector3 End, TVector3 Extent = TVector3(0,0,0));

	virtual void AddCollisionBody(class BThing* Thing);
	virtual void AddPrimitive(class BThing* Thing);
};

class TWorldOctreeNode
{
public:
	TWorldOctreeNode();

	TArray<class BThing*> Objects;

	unsigned int Size;
	int PositionX;
	int PositionY;
	int PositionZ;

	TWorldOctreeNode* Children[8];

	void AddThing(class BThing* Thing);
};

class UWorld
{
public:
	UWorld(BRenderer* R = NULL);
	~UWorld(void);

	inline void SetRenderer(BRenderer* R) { m_pRenderer = R; }

	bool InitializeWorld();
	bool DestroyWorld();

	virtual void Tick(DWORD dTime);

	TWorldStructure* m_pWorldData;

	CCameraViewport* m_pViewport;
	CCamera* m_pCamera;
	BRenderer* m_pRenderer;

	class CCharacter* m_Character;
	class CTerrain *m_Terrain;

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();

	virtual THitInfo LineCheck(TVector3 Start, TVector3 End, TVector3 Extent = TVector3(0,0,0));
};

extern UWorld* GWorld;