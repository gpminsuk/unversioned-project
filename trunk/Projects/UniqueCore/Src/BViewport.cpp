#include "StdAfx.h"
#include "BViewport.h"
#include "RResource.h"
#include <cstdlib>
#include <algorithm>

#include "BDriver.h"
#include "BThing.h"
#include "BComponent.h"
#include "BLineBatcher.h"
#include "BPrimitive.h"

BViewport::BViewport(void)
:	VisibleScenes(Scene_World|Scene_Collision) // TODO
{
}

BViewport::~BViewport(void)
{
}

void BViewport::SortTemplates()
{
}

void BViewport::Render(BComponent* pComponent)
{

}

void BViewport::Render(BThing* pThing)
{
	
}

void BViewport::Clear()
{
}