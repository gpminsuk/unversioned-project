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

void BViewport::Render(TPrimitiveTemplateBase* pTemplate, E_PrimitiveType RenderType)
{
	switch(RenderType)
	{
	case RT_OPAQUE:
		m_OpaquePrimitives.AddItem(pTemplate);
		m_Batches.m_pTemplates.AddItem(pTemplate);
		m_Batches.nVertexStride = pTemplate->pVertexBuffer->nVertexStride;
		m_Batches.nVertices += pTemplate->pVertexBuffer->nVertices;
		m_Batches.nIndices += pTemplate->pIndexBuffer->nIndices;
		//m_Batches.nVertices = 3;
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