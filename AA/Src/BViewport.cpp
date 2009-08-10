#include "StdAfx.h"
#include "BViewport.h"
#include "RResource.h"
#include <cstdlib>
#include <algorithm>

#include "BDriver.h"
#include "BPrimitive.h"

BViewport::BViewport(void)
{
}

BViewport::~BViewport(void)
{
}

void BViewport::SortTemplates()
{
}

void BViewport::Render(BPrimitive* pTemplate, E_PrimitiveType RenderType)
{
	switch(RenderType)
	{
	case RT_OPAQUE:
		m_OpaquePrimitives.AddItem(pTemplate);
		m_Batches.m_pTemplates.AddItem(pTemplate);
		for(unsigned int i=0;i<pTemplate->Primitives.Size();++i)
		{
			TPrimitive* Prim = pTemplate->Primitives(i);
			RSubMesh* Mesh = Prim->pSubMesh;
			m_Batches.nVertexStride = Mesh->pVB->nVertexStride;
			m_Batches.nVertices += Mesh->pVB->nVertices;
			m_Batches.nIndices += Mesh->pIB->nIndices;
		}
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