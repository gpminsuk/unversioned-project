#include "StdAfx.h"
#include "BViewport.h"
#include "RResource.h"
#include <cstdlib>
#include <algorithm>

#include "BDriver.h"
#include "BRenderer.h"
#include "BThing.h"
#include "BComponent.h"
#include "BLineBatcher.h"
#include "BPrimitive.h"
#include "BRenderingBatch.h"
#include "BCollisionBody.h"

#include "BRTRenderPass.h"

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

void BViewport::Remove(BPrimitive* pPrimitive)
{
	BatchManager->RemovePrimitive(pPrimitive);
}
void BViewport::Remove(BComponent* pComponent)
{
	for(unsigned int i=0;i<pComponent->Primitives.Size();++i)
	{
		Remove(pComponent->Primitives(i));
	}
}

void BViewport::Remove(BThing* pThing)
{
	for(unsigned int i=0;i<pThing->Components.Size();++i)
	{
		BComponent* pComponent = pThing->Components(i);
		for(unsigned int j=0;j<pComponent->Primitives.Size();++j)
		{
			Remove(pComponent->Primitives(j));
		}
	}
	for(unsigned int i=0;i<pThing->CollisionBodies.Size();++i)
	{
		BCollisionBody* pCollisionBody = pThing->CollisionBodies(i);
		for(unsigned int j=0;j<pCollisionBody->Primitives.Size();++j)
		{
			Remove(pCollisionBody->Primitives(j));
		}
	}
}

void BViewport::Render(BPrimitive* pPrimitive)
{
	BatchManager->AddPrimitive(pPrimitive);
}

void BViewport::Render(BComponent* pComponent)
{
	for(unsigned int i=0;i<pComponent->Primitives.Size();++i)
	{
		Render(pComponent->Primitives(i));
	}
}

void BViewport::Render(BThing* pThing)
{
	for(unsigned int i=0;i<pThing->Components.Size();++i)
	{
		BComponent* pComponent = pThing->Components(i);
		pComponent->RenderComponent(this);
	}
	for(unsigned int i=0;i<pThing->CollisionBodies.Size();++i)
	{
		BCollisionBody* pCollisionBody = pThing->CollisionBodies(i);
		for(unsigned int j=0;j<pCollisionBody->Primitives.Size();++j)
		{
			Render(pCollisionBody->Primitives(j));
		}
	}
}

void BViewport::RenderLight(BLightComponent* pLightComponent)
{
	m_Lights.AddItem(pLightComponent);
}

void BViewport::RemoveLight(BLightComponent* pLightComponent)
{
	m_Lights.DeleteItemByVal(pLightComponent);
}

void BViewport::RenderViewport()
{
	BatchManager->RenderBatchChunks(this);

	GBaseRTRenderPass->BeginPass(this);
	GBaseRTRenderPass->DrawPrimitive();
	GBaseRTRenderPass->EndPass();
}

void BViewport::Clear()
{
}