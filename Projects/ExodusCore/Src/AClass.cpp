#include "StdAfx.h"
#include "AClass.h"

#include "AObject.h"
IMPLEMENT_CLASS(AObject)

#include "AThread.h"
IMPLEMENT_CLASS(AThread)

#include "BThing.h"
IMPLEMENT_CLASS(BThing)

#include "BViewport.h"
IMPLEMENT_CLASS(BViewport)

#include "CWindowApp.h"
IMPLEMENT_CLASS(CWindowApp)

#include "CCameraViewport.h"
IMPLEMENT_CLASS(CCameraViewport)

#include "UWorld.h"
IMPLEMENT_CLASS(UWorld)

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
