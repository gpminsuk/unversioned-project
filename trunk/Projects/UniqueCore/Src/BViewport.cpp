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
#include "BRenderingBatch.h"

BViewport::BViewport(void)
:	VisibleScenes(Scene_World|Scene_Collision) // TODO
{
	BatchManager = new BRenderingBatchManager();
}

BViewport::~BViewport(void)
{
	delete BatchManager;
}

void BViewport::SortTemplates()
{
}

void BViewport::RemoveRender(BComponent* pComponent)
{
	for(unsigned int i=0;i<pComponent->Primitives.Size();++i)
	{
		BatchManager->RemovePrimitive(pComponent->Primitives(i));
	}
}

void BViewport::RemoveRender(BThing* pThing)
{
	for(unsigned int i=0;i<pThing->Components.Size();++i)
	{
		BComponent* pComponent = pThing->Components(i);
		for(unsigned int j=0;j<pComponent->Primitives.Size();++j)
		{
			BatchManager->RemovePrimitive(pComponent->Primitives(j));
		}
	}
}

void BViewport::Render(BComponent* pComponent)
{
	for(unsigned int i=0;i<pComponent->Primitives.Size();++i)
	{
		BatchManager->AddPrimitive(pComponent->Primitives(i));
	}
}

void BViewport::Render(BThing* pThing)
{
	for(unsigned int i=0;i<pThing->Components.Size();++i)
	{
		BComponent* pComponent = pThing->Components(i);
		for(unsigned int j=0;j<pComponent->Primitives.Size();++j)
		{
			BatchManager->AddPrimitive(pComponent->Primitives(j));
		}
	}
}

void BViewport::Clear()
{
}