#include "StdAfx.h"
#include "UWorld.h"

#include "BRenderer.h"
#include "BLineBatcher.h"

#include "BThing.h"

#include "BPrimitive.h"
#include "BCollisionBody.h"

#include "CCameraViewport.h"
#include "CTerrain.h"
#include "CCamera.h"
#include "CCharacter.h"
#include "CEmitter.h"
#include "CCylinderPrimitive.h"

UWorld* GWorld;

UWorld::UWorld(BRenderer* R)
: m_pRenderer(R),
	m_pViewport(0),
	m_pCamera(0)
{
	GWorld = this;
}

UWorld::~UWorld()
{
	//delete m_Emitter;
	delete m_Cylinder;
	delete m_Terrain;
	//delete m_Character;
	delete m_pWorldData;

	delete GLineBatcher;
}
float frame = 0;
void UWorld::Tick(DWORD dTime)
{
	m_pViewport->UpdateCameraViewport();

	m_pWorldData->Tick(dTime);
}

bool UWorld::InitializeWorld()
{
	GLineBatcher = new BLineBatcher();

	m_pWorldData = new TWorldOctree();

	m_pViewport = new CCameraViewport();

	m_pRenderer->AddViewport(m_pViewport);

	m_pCamera = new CCamera();

	m_pViewport->AddCamera(m_pCamera);
	m_pViewport->SetCurrentCamera(0);

	m_Terrain = new CTerrain(m_pCamera);
	m_pViewport->Render(m_Terrain->Primitives(0));
	m_pWorldData->AddThing(m_Terrain);

	//m_Emitter = new CEmitter();
	//m_pViewport->Render(m_Emitter->Primitives(0));
	//m_pWorldData->AddThing(m_Emitter);


	m_Character = new CCharacter();
	m_Character->SetCharacterPosition(TVector3(10.0f,10.0f,10.0f));
	m_pViewport->Render(m_Character->Primitives(0));
	m_pViewport->Render(m_Character->CollisionBodies(0)->Primitives(0));
	m_pWorldData->AddThing(m_Character);

	m_Cylinder = new CCylinderPrimitive();
	m_Cylinder->RenderType = RenderType_Opaque;
	m_Cylinder->Translation = TVector3(5,5,5);
	m_Cylinder->TM._41 = 5;
	m_Cylinder->TM._42 = 2.5;
	m_Cylinder->TM._43 = 5;
	m_pViewport->Render(m_Cylinder);
	
	m_pCamera->m_Subject = m_Character;
	return TRUE;
}

bool UWorld::DestroyWorld()
{
	delete m_pViewport;
	m_pViewport = 0;
	delete m_pCamera;
	m_pCamera = 0;
	return TRUE;
}

void UWorld::InputChar()
{
	m_pViewport->InputChar();
	m_pWorldData->InputChar();
}

void UWorld::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	m_pViewport->InputKey(Event, Param);
	m_pWorldData->InputKey(Event, Param);
}

void UWorld::InputMouse(EMouse_Event Event, TMouseInput_Param& Param)
{
	m_pViewport->InputMouse(Event, Param);
	m_pWorldData->InputMouse(Event, Param);
}

THitInfo UWorld::LineCheck(BThing* SourceThing, TVector3 Start, TVector3 End, TVector3 Extent)
{
	return m_pWorldData->LineCheck(SourceThing, Start, End, Extent);
}

/////////////////////////////////////////////////////// World Octree Structure /////////////////////////////////////////////////
TWorldOctreeNode::TWorldOctreeNode()
:	PositionX(0),
	PositionY(0),
	PositionZ(0)
{
	for(int i=0;i<8;++i)
		Children[i] = 0;
}

void TWorldOctreeNode::AddThing(class BThing* Thing)
{
	int Min, Max;
	int NewPositionX = 0, NewPositionY = 0, NewPositionZ = 0;
	unsigned int NewSize = 0, PowerOfTwo = 1, Size = 0;
	// X Size
	{
		Min = (int)(Thing->CollisionBodyBounds.Position.x - Thing->CollisionBodyBounds.Box.Extent.x - 1);
		Max = (int)(Thing->CollisionBodyBounds.Position.x + Thing->CollisionBodyBounds.Box.Extent.x + 1);
		Size = Max - Min;
		PowerOfTwo = 1;
		while(Size > PowerOfTwo)
			PowerOfTwo <<= 1;
		NewPositionX = Min;
		NewSize = PowerOfTwo;
	}
	// Y Size
	{
		Min = (int)(Thing->CollisionBodyBounds.Position.y - Thing->CollisionBodyBounds.Box.Extent.y - 1);
		Max = (int)(Thing->CollisionBodyBounds.Position.y + Thing->CollisionBodyBounds.Box.Extent.y + 1);
		Size = Max - Min;
		PowerOfTwo = 1;
		while(Size > PowerOfTwo)
			PowerOfTwo <<= 1;
		NewPositionY = Min;
		if(NewSize < PowerOfTwo) NewSize = PowerOfTwo;
	}
	// Z Size
	{
		Min = (int)(Thing->CollisionBodyBounds.Position.z - Thing->CollisionBodyBounds.Box.Extent.z - 1);
		Max = (int)(Thing->CollisionBodyBounds.Position.z + Thing->CollisionBodyBounds.Box.Extent.z + 1);
		Size = Max - Min;
		PowerOfTwo = 1;
		while(Size > PowerOfTwo)
			PowerOfTwo <<= 1;
		NewPositionZ = Min;
		NewSize = PowerOfTwo;
	}
	
	unsigned int TotalSize;
	// New X Size
	{
		TotalSize = ((PositionX>NewPositionX)?PositionX-NewPositionX:NewPositionX-PositionX)+Size;
		if(TotalSize < NewSize)
		{
			NewSize = TotalSize;
		}
		else
		{
			PowerOfTwo = 1;
			while(TotalSize > PowerOfTwo)
				PowerOfTwo <<= 1;
			NewSize = PowerOfTwo;
		}
	}
	// New Y Size
	{
		TotalSize = ((PositionY>NewPositionY)?PositionY-NewPositionY:NewPositionY-PositionY)+Size;
		if(TotalSize < NewSize)
		{
			NewSize = TotalSize;
		}
		else
		{
			PowerOfTwo = 1;
			while(TotalSize > PowerOfTwo)
				PowerOfTwo <<= 1;
			NewSize = PowerOfTwo;
		}
	}
	// New Z Size
	{
		TotalSize = ((PositionZ>NewPositionZ)?PositionZ-NewPositionZ:NewPositionZ-PositionZ)+Size;
		if(TotalSize < NewSize)
		{
			NewSize = TotalSize;
		}
		else
		{
			PowerOfTwo = 1;
			while(TotalSize > PowerOfTwo)
				PowerOfTwo <<= 1;
			NewSize = PowerOfTwo;
		}
	}
	Size = NewSize;
	PositionX = (PositionX<NewPositionX)?PositionX:NewPositionX;
	PositionY = (PositionY<NewPositionY)?PositionY:NewPositionY;
	PositionZ = (PositionZ<NewPositionZ)?PositionZ:NewPositionZ;

	Objects.AddItem(Thing);
}

TWorldOctree::TWorldOctree()
:	CollisionBodyRootNode(0),
	PrimitiveRootNode(0)
{
	
}

TWorldOctree::~TWorldOctree()
{
	delete CollisionBodyRootNode;
	delete PrimitiveRootNode;
}

void TWorldOctree::Tick(unsigned long dTime)
{
	for(unsigned int i=0;i<AllObjects.Size();++i)
	{
		AllObjects(i)->Tick(dTime);
	}
	for(unsigned int i=0;i<AllObjects.Size();++i)
	{
		AllObjects(i)->PhysicsTick(dTime);
	}
}

void TWorldOctree::AddThing(BThing* Thing)
{
	AllObjects.AddItem(Thing);
	AddCollisionBody(Thing);
	AddPrimitive(Thing);
}

THitInfo TWorldOctree::LineCheck(BThing* SourceThing, TVector3& Start, TVector3& End, TVector3& Extent)
{
	THitInfo HitInfo;
	for(unsigned int i = 0;i<CollisionBodyRootNode->Objects.Size();++i)
	{
		if(CollisionBodyRootNode->Objects(i) !=  SourceThing)
			HitInfo.HitPosition = CollisionBodyRootNode->Objects(i)->LineCheck(Start, End, Extent);
	}
	return HitInfo;
}

void TWorldOctree::AddCollisionBody(BThing* Thing)
{
	if(CollisionBodyRootNode)
	{
		CollisionBodyRootNode->AddThing(Thing);
	}
	else
	{
		CollisionBodyRootNode = new TWorldOctreeNode();
		CollisionBodyRootNode->Objects.AddItem(Thing);
		int Min, Max;
		unsigned int Size, PowerOfTwo;
		// X Size
		{
			Min = (int)(Thing->CollisionBodyBounds.Position.x - Thing->CollisionBodyBounds.Box.Extent.x - 1);
			Max = (int)(Thing->CollisionBodyBounds.Position.x + Thing->CollisionBodyBounds.Box.Extent.x + 1);
			Size = Max - Min;
			PowerOfTwo = 1;
			while(Size > PowerOfTwo)
				PowerOfTwo <<= 1;
			CollisionBodyRootNode->PositionX = Min;
			CollisionBodyRootNode->Size = PowerOfTwo;
		}
		// Y Size
		{
			Min = (int)(Thing->CollisionBodyBounds.Position.y - Thing->CollisionBodyBounds.Box.Extent.y - 1);
			Max = (int)(Thing->CollisionBodyBounds.Position.y + Thing->CollisionBodyBounds.Box.Extent.y + 1);
			Size = Max - Min;
			PowerOfTwo = 1;
			while(Size > PowerOfTwo)
				PowerOfTwo <<= 1;
			CollisionBodyRootNode->PositionY = Min;
			if(CollisionBodyRootNode->Size < PowerOfTwo) CollisionBodyRootNode->Size = PowerOfTwo;
		}
		// Z Size
		{
			Min = (int)(Thing->CollisionBodyBounds.Position.z - Thing->CollisionBodyBounds.Box.Extent.z - 1);
			Max = (int)(Thing->CollisionBodyBounds.Position.z + Thing->CollisionBodyBounds.Box.Extent.z + 1);
			Size = Max - Min;
			PowerOfTwo = 1;
			while(Size > PowerOfTwo)
				PowerOfTwo <<= 1;
			CollisionBodyRootNode->PositionZ = Min;
			CollisionBodyRootNode->Size = PowerOfTwo;
		}
	}
}

void TWorldOctree::AddPrimitive(BThing* Thing)
{
}

void TWorldOctree::RemoveThing(BThing* Thing)
{
	AllObjects.DeleteItemByVal(Thing);
}

void TWorldOctree::InputMouse(EMouse_Event Event, TMouseInput_Param& Param)
{
	for(unsigned int i=0;i<AllObjects.Size();++i)
	{
		AllObjects(i)->InputMouse(Event, Param);
	}
}

void TWorldOctree::InputKey(EKey_Event Event, TKeyInput_Param& Param)
{
	for(unsigned int i=0;i<AllObjects.Size();++i)
	{
		AllObjects(i)->InputKey(Event, Param);
	}
}

void TWorldOctree::InputChar()
{
	for(unsigned int i=0;i<AllObjects.Size();++i)
	{
		AllObjects(i)->InputChar();
	}
}