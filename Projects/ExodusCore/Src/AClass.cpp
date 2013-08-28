#include "StdAfx.h"
#include "AClass.h"

#include "AObject.h"
IMPLEMENT_CLASS(AObject)

#include "AThread.h"
IMPLEMENT_CLASS(AThread)

#include "BThing.h"
IMPLEMENT_CLASS(BThing)

#include "CWindowsApplication.h"
IMPLEMENT_CLASS(CWindowsApplication)

#include "UWorld.h"
IMPLEMENT_CLASS(UWorld)

#include "CMeshPrimitive.h"
IMPLEMENT_CLASS(CMeshPrimitive)

#include "RMesh.h"
IMPLEMENT_CLASS(RMesh);
IMPLEMENT_CLASS(RSubMesh);

#include "RBoneHierarchy.h"
IMPLEMENT_CLASS(RBoneHierarchy);
IMPLEMENT_CLASS(RBone);

#include "CCharacter.h"
IMPLEMENT_CLASS(CCharacter);

AClassManager* GClassManager;

void AClassManager::AddClass(AClass* Class) {
    Classes.AddItem(Class);
}

AClass::AClass(TString ClassName)
    :
    ClassName(ClassName),
    Constructor(0) {
    if (!GClassManager) {
        GClassManager = new AClassManager();
    }
    GClassManager->AddClass(this);
}

AClass::~AClass() {
    if (Constructor) {
        delete Constructor;
    }
    if (GClassManager) {
        delete GClassManager;
        GClassManager = 0;
    }
}
