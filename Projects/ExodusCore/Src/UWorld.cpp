#include "StdAfx.h"
#include "UWorld.h"

#include "BRenderer.h"
#include "BLineBatcher.h"
#include "BRenderPass.h"
#include "BThing.h"

#include "BPrimitive.h"
#include "BCollisionBody.h"
#include "BRenderingBatch.h"

#include "CTerrain.h"
#include "BCamera.h"
#include "CCharacter.h"
#include "CEmitter.h"
#include "CCylinderPrimitive.h"
#include "CWindowsViewport.h"

UWorld* GWorld;

UWorld::UWorld() {
	BatchManager = new BRenderingBatchManager();
	GLineBatcher = new BLineBatcher();
	m_pWorldData = new TWorldOctree();
	GWorld = this;
}

UWorld::~UWorld() {
    //delete m_Emitter;
//	delete m_Cylinder;
    //delete m_Terrain;
    //delete m_Character;
    delete m_pWorldData;
    //delete m_Button;

    //delete GLineBatcher;
}

bool UWorld::Access(AAccessor& Accessor) {
	__super::Access(Accessor);

	Accessor << ((TWorldOctree*)m_pWorldData)->AllObjects;
	return true;
}

bool UWorld::Tick(unsigned long dTime) {
	m_pWorldData->Tick(dTime);
	for(unsigned int i=0;i<Viewports.Size();++i) {
		BViewport* Viewport = Viewports(i);
		Viewport->UpdateViewport();
	}
    return true;
}

void UWorld::RemoveThing(BThing* Thing) {
	m_pRenderer->Render(BatchManager, Thing);
    m_pWorldData->RemoveThing(Thing);
}

void UWorld::AddThing(BThing* Thing) {
	m_pRenderer->Render(BatchManager, Thing);
    m_pWorldData->AddThing(Thing);
}

bool UWorld::DestroyWorld() {
    return TRUE;
}

void UWorld::InputChar() {
    m_pWorldData->InputChar();
}

void UWorld::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
    m_pWorldData->InputKey(Event, Param);
}

void UWorld::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
    m_pWorldData->InputMouse(Event, Param);
	switch(Event) {
	case MOUSE_LeftButtonDown:
		THitInfo Info = LineCheck(0, TVector3(30,100,30), TVector3(30,-100,30));
		if(Info.HitPosition != TVector3(0,0,0)) {
			int i=0;
			i=1;
		}
		break;
	}
}

THitInfo UWorld::LineCheck(BThing* SourceThing, TVector3 Start, TVector3 End, TVector3 Extent) {
    return m_pWorldData->LineCheck(SourceThing, Start, End, Extent);
}

BViewport* UWorld::CreateViewport(TViewportInfo& Info) {
	BViewport* Viewport = new CWindowsViewport(Info.m_wWidth, Info.m_wHeight, Info.ProjectionType, Info.RenderMode, Info.CameraMode, Info.m_hWnd);
	Viewports.AddItem(Viewport);
	return Viewport;
}

void UWorld::RemoveViewport(BViewport* Viewport) {
	Viewports.DeleteItemByVal(Viewport);
}

void UWorld::OnViewportsResized() {
	if(Viewports.Size() == 0) {
		return;
	}
	unsigned int MaxWidth = 0;
	unsigned int MaxHeight = 0;
	for(unsigned int i=0;i<Viewports.Size();++i) {
		Viewports(i)->OnSizeChanged();
		if(MaxWidth < Viewports(i)->Width) {
			MaxWidth = Viewports(i)->Width;
		}
		if(MaxHeight < Viewports(i)->Height) {
			MaxHeight = Viewports(i)->Height;
		}
	}
	if(m_pRenderer) {
		GRenderPassResource.Initialize(MaxWidth, MaxHeight);
	}
}

BViewport* UWorld::FindViewport(HWND hWnd) {
	for(unsigned int i=0;i<Viewports.Size();++i) {
		CWindowsViewport* Viewport = (CWindowsViewport*)Viewports(i);
		if(Viewport->Handle == hWnd) {
			return Viewport;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////// World Octree Structure /////////////////////////////////////////////////
TWorldOctreeNode::TWorldOctreeNode()
    :
    PositionX(0),
    PositionY(0),
    PositionZ(0) {
    for (int i = 0; i < 8; ++i)
        Children[i] = 0;
}

void TWorldOctreeNode::RemoveThing(class BThing* Thing) {
    Objects.DeleteItemByVal(Thing);
}

void TWorldOctreeNode::AddThing(class BThing* Thing) {
    int Min, Max;
    int NewPositionX = 0, NewPositionY = 0, NewPositionZ = 0;
    unsigned int NewSize = 0, PowerOfTwo = 1, Size = 0;
    // X Size
    {
        Min = (int) (Thing->CollisionBodyBounds.Position.x
                     - Thing->CollisionBodyBounds.Box.Extent.x - 1);
        Max = (int) (Thing->CollisionBodyBounds.Position.x
                     + Thing->CollisionBodyBounds.Box.Extent.x + 1);
        Size = Max - Min;
        PowerOfTwo = 1;
        while (Size > PowerOfTwo)
            PowerOfTwo <<= 1;
        NewPositionX = Min;
        NewSize = PowerOfTwo;
    }
    // Y Size
    {
        Min = (int) (Thing->CollisionBodyBounds.Position.y
                     - Thing->CollisionBodyBounds.Box.Extent.y - 1);
        Max = (int) (Thing->CollisionBodyBounds.Position.y
                     + Thing->CollisionBodyBounds.Box.Extent.y + 1);
        Size = Max - Min;
        PowerOfTwo = 1;
        while (Size > PowerOfTwo)
            PowerOfTwo <<= 1;
        NewPositionY = Min;
        if (NewSize < PowerOfTwo)
            NewSize = PowerOfTwo;
    }
    // Z Size
    {
        Min = (int) (Thing->CollisionBodyBounds.Position.z
                     - Thing->CollisionBodyBounds.Box.Extent.z - 1);
        Max = (int) (Thing->CollisionBodyBounds.Position.z
                     + Thing->CollisionBodyBounds.Box.Extent.z + 1);
        Size = Max - Min;
        PowerOfTwo = 1;
        while (Size > PowerOfTwo)
            PowerOfTwo <<= 1;
        NewPositionZ = Min;
        NewSize = PowerOfTwo;
    }

    unsigned int TotalSize;
    // New X Size
    {
        TotalSize = (
                        (PositionX > NewPositionX) ? PositionX - NewPositionX :
                        NewPositionX - PositionX) + Size;
        if (TotalSize < NewSize) {
            NewSize = TotalSize;
        } else {
            PowerOfTwo = 1;
            while (TotalSize > PowerOfTwo)
                PowerOfTwo <<= 1;
            NewSize = PowerOfTwo;
        }
    }
    // New Y Size
    {
        TotalSize = (
                        (PositionY > NewPositionY) ? PositionY - NewPositionY :
                        NewPositionY - PositionY) + Size;
        if (TotalSize < NewSize) {
            NewSize = TotalSize;
        } else {
            PowerOfTwo = 1;
            while (TotalSize > PowerOfTwo)
                PowerOfTwo <<= 1;
            NewSize = PowerOfTwo;
        }
    }
    // New Z Size
    {
        TotalSize = (
                        (PositionZ > NewPositionZ) ? PositionZ - NewPositionZ :
                        NewPositionZ - PositionZ) + Size;
        if (TotalSize < NewSize) {
            NewSize = TotalSize;
        } else {
            PowerOfTwo = 1;
            while (TotalSize > PowerOfTwo)
                PowerOfTwo <<= 1;
            NewSize = PowerOfTwo;
        }
    }
    Size = NewSize;
    PositionX = (PositionX < NewPositionX) ? PositionX : NewPositionX;
    PositionY = (PositionY < NewPositionY) ? PositionY : NewPositionY;
    PositionZ = (PositionZ < NewPositionZ) ? PositionZ : NewPositionZ;

    Objects.AddItem(Thing);
}

TWorldOctree::TWorldOctree()
    :
    CollisionBodyRootNode(0),
    PrimitiveRootNode(0) {

}

TWorldOctree::~TWorldOctree() {
    delete CollisionBodyRootNode;
    delete PrimitiveRootNode;
}

bool TWorldOctree::Tick(unsigned long dTime) {	
    for (unsigned int i = 0; i < AllObjects.Size(); ++i) {
        AllObjects(i)->Tick(dTime);
    }
    for (unsigned int i = 0; i < AllObjects.Size(); ++i) {
        AllObjects(i)->PhysicsTick(dTime);
    }
    return true;
}

void TWorldOctree::AddThing(BThing* Thing) {
    AllObjects.AddItem(Thing);
    AddCollisionBody(Thing);
    AddPrimitive(Thing);
}

THitInfo TWorldOctree::LineCheck(BThing* SourceThing, TVector3& Start, TVector3& End, TVector3& Extent) {
    THitInfo HitInfo;
    for (unsigned int i = 0; i < CollisionBodyRootNode->Objects.Size(); ++i) {
        if (CollisionBodyRootNode->Objects(i) != SourceThing) {
            HitInfo.HitPosition = CollisionBodyRootNode->Objects(i)->LineCheck(
                                      Start, End, Extent);
            if (HitInfo.HitPosition != TVector3(0, 0, 0)) {
                return HitInfo;
            }
        }
    }
    return HitInfo;
}

void TWorldOctree::RemoveCollisionBody(BThing* Thing) {
    if (CollisionBodyRootNode) {
        CollisionBodyRootNode->RemoveThing(Thing);
    } else {
        CollisionBodyRootNode = new TWorldOctreeNode();
        CollisionBodyRootNode->Objects.DeleteItemByVal(Thing);
    }
}

void TWorldOctree::AddCollisionBody(BThing* Thing) {
    if (CollisionBodyRootNode) {
        CollisionBodyRootNode->AddThing(Thing);
    } else {
        CollisionBodyRootNode = new TWorldOctreeNode();
        CollisionBodyRootNode->Objects.AddItem(Thing);
        int Min, Max;
        unsigned int Size, PowerOfTwo;
        // X Size
        {
            Min = (int) (Thing->CollisionBodyBounds.Position.x
                         - Thing->CollisionBodyBounds.Box.Extent.x - 1);
            Max = (int) (Thing->CollisionBodyBounds.Position.x
                         + Thing->CollisionBodyBounds.Box.Extent.x + 1);
            Size = Max - Min;
            PowerOfTwo = 1;
            while (Size > PowerOfTwo)
                PowerOfTwo <<= 1;
            CollisionBodyRootNode->PositionX = Min;
            CollisionBodyRootNode->Size = PowerOfTwo;
        }
        // Y Size
        {
            Min = (int) (Thing->CollisionBodyBounds.Position.y
                         - Thing->CollisionBodyBounds.Box.Extent.y - 1);
            Max = (int) (Thing->CollisionBodyBounds.Position.y
                         + Thing->CollisionBodyBounds.Box.Extent.y + 1);
            Size = Max - Min;
            PowerOfTwo = 1;
            while (Size > PowerOfTwo)
                PowerOfTwo <<= 1;
            CollisionBodyRootNode->PositionY = Min;
            if (CollisionBodyRootNode->Size < PowerOfTwo)
                CollisionBodyRootNode->Size = PowerOfTwo;
        }
        // Z Size
        {
            Min = (int) (Thing->CollisionBodyBounds.Position.z
                         - Thing->CollisionBodyBounds.Box.Extent.z - 1);
            Max = (int) (Thing->CollisionBodyBounds.Position.z
                         + Thing->CollisionBodyBounds.Box.Extent.z + 1);
            Size = Max - Min;
            PowerOfTwo = 1;
            while (Size > PowerOfTwo)
                PowerOfTwo <<= 1;
            CollisionBodyRootNode->PositionZ = Min;
            CollisionBodyRootNode->Size = PowerOfTwo;
        }
    }
}

void TWorldOctree::AddPrimitive(BThing* Thing) {
}

void TWorldOctree::RemoveThing(BThing* Thing) {
    AllObjects.DeleteItemByVal(Thing);
}

void TWorldOctree::InputMouse(EMouse_Event Event, TMouseInput_Param& Param) {
    for (unsigned int i = 0; i < AllObjects.Size(); ++i) {
        AllObjects(i)->InputMouse(Event, Param);
    }
}

void TWorldOctree::InputKey(EKey_Event Event, TKeyInput_Param& Param) {
    for (unsigned int i = 0; i < AllObjects.Size(); ++i) {
        AllObjects(i)->InputKey(Event, Param);
    }
}

void TWorldOctree::InputChar() {
    for (unsigned int i = 0; i < AllObjects.Size(); ++i) {
        AllObjects(i)->InputChar();
    }
}
