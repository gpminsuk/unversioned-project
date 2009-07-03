#include "StdAfx.h"
#include "BViewport.h"
#include "RResource.h"
#include <cstdlib>
#include <algorithm>

BViewport::BViewport(void)
{
}

BViewport::~BViewport(void)
{
}

void BViewport::SortTemplates()
{
}

int BViewport::PrimitiveCompare(TPrimitiveTemplateBase *tb1, TPrimitiveTemplateBase *tb2)
{
	if(tb1->pMaterial->TID == tb2->pMaterial->TID)
		return tb1->pVertexBuffer->nVertices < tb2->pVertexBuffer->nVertices;
	else
		return tb1->pMaterial->TID < tb2->pMaterial->TID;	
}

void BViewport::Render(TPrimitiveTemplateBase* pTemplate, E_RenderType RenderType)
{
	switch(RenderType)
	{
	case RT_BASE_PASS:
		m_OpaquePrimitives.AddItem(pTemplate);
		m_Batches.m_pTemplates.AddItem(pTemplate);
		m_Batches.nVertexStride = pTemplate->pVertexBuffer->nVertexStride;
		m_Batches.nVertices += pTemplate->pVertexBuffer->nVertices;
	}
}

void BViewport::Clear()
{
}

void BViewport::operator =(BViewport& vp)
{
	m_OpaquePrimitives			=		vp.m_OpaquePrimitives;
	m_Batches					=		vp.m_Batches;
	m_ProjectionMatrix			=		vp.m_ProjectionMatrix;
	m_TranslucentPrimitives		=		vp.m_TranslucentPrimitives;
	m_ViewMatrix				=		vp.m_ViewMatrix;
}