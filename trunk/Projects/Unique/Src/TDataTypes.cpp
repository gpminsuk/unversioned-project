#include "stdafx.h"
#include "TDataTypes.h"
#include "RResource.h"

#include "BPrimitive.h"

TMatrix TMatrix::Identity;

int TBatch::GetNumIndices()
{
	int NumIndices = 0;
	for(unsigned int i=0;i<m_pTemplates.Size();++i)
	{
		NumIndices += m_pTemplates(i)->GetNumIndices();
	}
	return NumIndices;
}

void TBatch::IndexTessellate()
{
	for(unsigned int i=0;i<m_pTemplates.Size();++i)
	{
		m_pTemplates(i)->IndexTessellate();
	}
}