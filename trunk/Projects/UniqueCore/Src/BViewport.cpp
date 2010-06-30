#include "StdAfx.h"
#include "BViewport.h"
#include "RResource.h"
#include <cstdlib>
#include <algorithm>

#include "BDriver.h"
#include "BThing.h"
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

void BViewport::Render(BThing* pThing)
{
	TArray<BPrimitive*> pTemplates = pThing->GetPrimitives();
	for(unsigned int i=0;i<pTemplates.Size();++i)
	{
		BPrimitive* pTemplate = pTemplates(i);
		switch(pTemplate->RenderType)
		{
		case RenderType_Opaque:
			{
				m_OpaquePrimitives.AddItem(pTemplate);
				m_Batches.m_pTemplates.AddItem(pTemplate);
				BPrimitive* Prim = pTemplate;
				Prim->Render(&m_Batches);
				m_Batches.RenderType = PrimitiveType_TriangleList;
			}
			break;
		case RenderType_Line:
			{
				m_LineBatch.m_pTemplates.AddItem(pTemplate);
				BPrimitive* Prim = pTemplate;
				Prim->Render(&m_LineBatch);
				m_LineBatch.RenderType = PrimitiveType_LineList;
			}
			break;
		case RenderType_Particle:
			{
				m_ParticleBatch.m_pTemplates.AddItem(pTemplate);
				BPrimitive* Prim = pTemplate;
				Prim->Render(&m_ParticleBatch);
				m_ParticleBatch.RenderType = PrimitiveType_TriangleList;
			}
			break;
		}
	}	
}

void BViewport::RenderUIPane(BUIPane* pUIPane)
{
	m_UIPanes.AddItem(pUIPane);
}

void BViewport::Clear()
{
}

void BViewport::operator =(BViewport& vp)
{
	m_OpaquePrimitives			=		vp.m_OpaquePrimitives;
	m_Batches					=		vp.m_Batches;
	m_LineBatch					=		vp.m_LineBatch;
	m_ParticleBatch				=		vp.m_ParticleBatch;
	m_ProjectionMatrix			=		vp.m_ProjectionMatrix;
	m_TranslucentPrimitives		=		vp.m_TranslucentPrimitives;
	m_ViewMatrix				=		vp.m_ViewMatrix;
	m_Height					=		vp.m_Height;
	m_Width						=		vp.m_Width;
	VisibleScenes				=		vp.VisibleScenes;
}