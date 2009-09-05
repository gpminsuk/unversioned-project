#include "StdAfx.h"
#include "BViewport.h"
#include "RResource.h"
#include <cstdlib>
#include <algorithm>

#include "BDriver.h"
#include "BPrimitive.h"
#include "BLineBatcher.h"

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

void BViewport::Render(BPrimitive* pTemplate)
{
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
	}
}

void BViewport::Clear()
{
}

void BViewport::operator =(BViewport& vp)
{
	m_OpaquePrimitives			=		vp.m_OpaquePrimitives;
	m_Batches					=		vp.m_Batches;
	m_LineBatch					=		vp.m_LineBatch;
	m_ProjectionMatrix			=		vp.m_ProjectionMatrix;
	m_TranslucentPrimitives		=		vp.m_TranslucentPrimitives;
	m_ViewMatrix				=		vp.m_ViewMatrix;
	m_Height					=		vp.m_Height;
	m_Width						=		vp.m_Width;
	VisibleScenes				=		vp.VisibleScenes;
}