#pragma once

#include <windows.h>
#include "AObject.h"
#include "TDataTypes.h"
#include "InputDefine.h"
#include "RAsset.h"

#include "UFreeTypeDrawer.h"

class BRenderingBatchManager;
class BRenderer;
class BThing;

class CCamera;
class CCameraViewport;

class UWorldTemplate : public AObject {
public:
	virtual void Create(class UWorld* World) = 0;
};

class THitInfo
{
public:
	class BThing* Thing;
	TVector3 HitPosition;
};

struct TViewportInfo
{
public:
	enum ECameraMode CameraMode;
	enum EViewportRenderMode RenderMode;
	enum EViewportProjectionType ProjectionType;

	HWND m_hWnd;

	int m_wWidth;
	int m_wHeight;
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

class UWorld: public RAsset
{
DECLARE_CLASS(UWorld,)
	public:
	UWorld();
	~UWorld(void);

	inline void SetRenderer(BRenderer* R) {
		m_pRenderer = R;
	}

	virtual bool DestroyWorld();
	
	virtual bool Access(AAccessor& Accessor);
	virtual bool Tick(unsigned long dTime);
	
	TWorldStructure* m_pWorldData;
	BRenderer* m_pRenderer;
	BRenderingBatchManager* BatchManager;
	TArray<BViewport*> Viewports;

	BViewport* CreateViewport(TViewportInfo& Info);
	void RemoveViewport(BViewport* Viewport);
	BViewport* FindViewport(HWND hWnd);
	void OnViewportsResized();

	virtual void InputMouse(EMouse_Event Event, TMouseInput_Param& Param);
	virtual void InputKey(EKey_Event Event, TKeyInput_Param& Param);
	virtual void InputChar();

	virtual THitInfo LineCheck(BThing* SourceThing, TVector3 Start, TVector3 End, TVector3 Extent =
			TVector3(0, 0, 0));
	void AddThing(BThing* Thing);
	void RemoveThing(BThing* Thing);

	virtual TArray<TString> GetCreatablePrimitiveClassNames()
	{
		TArray<TString> Ret;
		return Ret;
	}
	protected:
};
