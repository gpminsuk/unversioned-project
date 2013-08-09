#pragma once

#include <windows.h>
#include "AObject.h"
#include "TDataTypes.h"
#include "InputDefine.h"

#include "UFreeTypeDrawer.h"

class BRenderer;
class BThing;

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
	TWorldStructure() {
	}
	virtual ~TWorldStructure() {
	}

	virtual bool Tick(unsigned long dTime) = 0;
	virtual void AddThing(class BThing* Thing) = 0;
	virtual void RemoveThing(class BThing* Thing) = 0;

	virtual THitInfo LineCheck(class BThing* SourceThing, TVector3& Start, TVector3& End, TVector3& Extent =
			TVector3(0, 0, 0)) = 0;

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param) = 0;
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param) = 0;
	virtual void InputChar() = 0;
};

class TWorldOctree: public TWorldStructure
{
public:
	TWorldOctree();
	~TWorldOctree();

	class TWorldOctreeNode* CollisionBodyRootNode;
	class TWorldOctreeNode* PrimitiveRootNode;

	TArray<class BThing*> AllObjects;

	virtual bool Tick(unsigned long dTime);
	virtual void AddThing(class BThing* Thing);
	virtual void RemoveThing(class BThing* Thing);
	virtual THitInfo LineCheck(class BThing* SourceThing, TVector3& Start, TVector3& End, TVector3& Extent =
			TVector3(0, 0, 0));

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();

	virtual void AddCollisionBody(class BThing* Thing);
	virtual void AddPrimitive(class BThing* Thing);

	virtual void RemoveCollisionBody(class BThing* Thing);
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
	void RemoveThing(class BThing* Thing);
};

class UWorld: public AObject
{
DECLARE_CLASS(UWorld,)
	public:
	UWorld();
	~UWorld(void);

	inline void SetRenderer(BRenderer* R) {
		m_pRenderer = R;
	}

	virtual bool InitializeWorld();
	virtual bool DestroyWorld();

	virtual bool Tick(unsigned long dTime);

	TWorldStructure* m_pWorldData;
	BRenderer* m_pRenderer;

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();

	virtual THitInfo LineCheck(BThing* SourceThing, TVector3 Start, TVector3 End, TVector3 Extent =
			TVector3(0, 0, 0));
	void AddThing(BThing* Thing);
	void RemoveThing(BThing* Thing);
	protected:

};

extern UWorld* GWorld;
