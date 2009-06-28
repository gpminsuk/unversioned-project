#include "StdAfx.h"
#include "BViewport.h"
#include "RResource.h"
#include <cstdlib>
#include <algorithm>

BViewport::BViewport(void)
:	m_BasePrimitives(0),
	m_nBasePrimitives(0),
	m_AlphaPrimitives(0),
	m_nAlphaPrimitives(0)
{
	m_BasePrimitives = new TPrimitiveTemplateBase*[10];
	m_nBasePrimitives = 10;
}

BViewport::~BViewport(void)
{
	delete[] m_BasePrimitives;
}

void BViewport::SortTemplates()
{
	return;
	std::sort(m_BasePrimitives, m_BasePrimitives + m_nBasePrimitives, BViewport::PrimitiveCompare);
	std::sort(m_AlphaPrimitives, m_AlphaPrimitives + m_nAlphaPrimitives, BViewport::PrimitiveCompare);
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
		m_BasePrimitives[0] = pTemplate;
		m_Batches.m_pTemplates.AddItem(pTemplate);
		m_Batches.nVertexStride = pTemplate->pVertexBuffer->nVertexStride;
		m_Batches.nVertices += pTemplate->pVertexBuffer->nVertices;
	}
}

void BViewport::Clear()
{
}